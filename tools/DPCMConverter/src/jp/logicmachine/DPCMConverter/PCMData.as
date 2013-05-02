package jp.logicmachine.DPCMConverter {
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	public class PCMData {
		
		private var m_channels:int = 0;
		private var m_sampleRate:int = 0;
		private var m_blockSize:int = 0;
		private var m_bitdepth:int = 0;
		private var m_samplesNum:int = 0;
		private var m_pcmData:Vector.<Vector.<Number>> = null;

		public function PCMData() {
		}
		
		public function get channels():int {
			return m_channels;
		}
		public function get sampleRate():int {
			return m_sampleRate;
		}
		public function get samplesNum():int {
			return m_samplesNum;
		}
		public function get data():Vector.<Vector.<Number>> {
			return m_pcmData;
		}
		
		public function fromWaveFile(data:ByteArray):Boolean {
			data.position = 0;
			data.endian = Endian.LITTLE_ENDIAN;
 
			// RIFFヘッダの確認
			if(data.readMultiByte(4, "us-ascii") != "RIFF"){
				throw new Error("対応していないフォーマットのファイルです");
			}
			var filesize:int = data.readUnsignedInt();

			// WAVEヘッダの確認
			if(data.readMultiByte(4, "us-ascii") != "WAVE"){
				throw new Error("対応していないフォーマットのファイルです");
			}
			
			var chunkSize:int;
			// fmtチャンクを探す
			while(true){
				if(data.readMultiByte(4, "us-ascii") == "fmt "){
					if(data.readUnsignedInt() != 16){
						throw new Error("対応していないフォーマットのファイルです");
					}

					// リニアPCMしか食えないよ!
					if(data.readUnsignedShort() != 1){
						throw new Error("対応していないフォーマットのファイルです");
					}
					m_channels = data.readUnsignedShort();	// チャンネル数
					m_sampleRate = data.readUnsignedInt();	// サンプルレート
					data.readUnsignedInt();					// データ速度は読み飛ばす
					m_blockSize = data.readUnsignedShort();	// 1ブロックのサイズ
					m_bitdepth = data.readUnsignedShort();;	// サンプルのビット幅
					if(m_bitdepth != 8 && m_bitdepth != 16){
						throw new Error("対応していないフォーマットのファイルです");
					}
					break;
				}else{
					chunkSize = data.readUnsignedInt();
					data.position += chunkSize;
				}
			}
			
			// dataチャンクを探す
			while(true){
				if(data.readMultiByte(4, "us-ascii") == "data"){
					// バッファの確保
					var samplesNum:int = data.readUnsignedInt() / m_blockSize;
					m_samplesNum = samplesNum;
					m_pcmData = new Vector.<Vector.<Number>>(m_channels);
					for(var i:int = 0; i < m_channels; i++){
						m_pcmData[i] = new Vector.<Number>(samplesNum);
					}
					
					for(var position:int = 0; position < samplesNum; position++){
						for(var ch:int = 0; ch < m_channels; ch++){
							if(m_bitdepth == 8){
								m_pcmData[ch][position] = (Number(data.readUnsignedByte() - 0x80) / 0x80);
							}else if(m_bitdepth == 16){
								m_pcmData[ch][position] = (Number(data.readShort()) / 0x8000);
							}
						}
					}
					break;
				}else{
					chunkSize = data.readUnsignedInt();
					data.position += chunkSize;
				}
			}
			
			return true;
		}
		
		private function getUInt16(data:ByteArray, offset:int):int {
			if(data.length < (offset + 2)){
				throw new Error("対応していないフォーマットのファイルです");
			}
			return (data[offset] << 8) + data[offset + 1];
		}
		private function getSInt16(data:ByteArray, offset:int):int {
			if(data.length < (offset + 2)){
				throw new Error("対応していないフォーマットのファイルです");
			}
			return (data[offset] << 8) + data[offset + 1];
		}
		private function getUInt32(data:ByteArray, offset:int):int {
			if(data.length < (offset + 4)){
				throw new Error("対応していないフォーマットのファイルです");
			}
			return (data[offset] << 24) + (data[offset + 1] << 16) + (data[offset + 2] << 8) + data[offset + 3];
		}
		private function checkMagic(data:ByteArray, offset:int, magic:String):Boolean {
			if(data.length < (offset + magic.length)){
				throw new Error("対応していないフォーマットのファイルです");
			}
			for(var i:int = 0; i < magic.length; i++){
				if(data[offset + i] != magic.charAt(i)){
					return false;
				}
			}
			return true;
		}

	}

}