/**
 * リクナビ(2010)の説明会予約状況を取得、予約可能性を通知するLimeChatスクリプト。
 * 
 * あらかじめ指定しておいたページを定期的に受信して日程表を読み、
 * 指定の項目の受付締切が「満席」かどうかを判定して処理をさせます。
 * 
 * 監視間隔はスクリプト中に設定があります。この間隔ごとにスクリプトは
 * 1項目のみのチェックをおこないます。すなわち、1000個の説明会の予約状況を
 * 監視対象として登録している場合、チェック間隔を1分に設定しても、それら
 * すべてを一周するのには1000分かかるので、チェック頻度は低くなります。
 * サーバに優しい仕様ということで、どうかご理解いただきたいところです。
 * 
 * スクリプトを動作させているサーバのどこかで .rikunabi というメッセージを
 * 投稿すると、それに反応してスクリプトが手動で動作します（動作主発言にのみ反応）。
 * 手動確認はテスト目的で作られたものと言っても過言ではありません。確認中に
 * 重ねて確認処理が起こることはやや動作に不具合を生じる可能性があります。
 * くれぐれも手動確認機能を濫用なさらないことをおすすめします。
 * 
 * LimeChatの仕様上、スクリプトを上書き保存すると、スクリプトはリロードされて
 * 最初から実行し直されます。本稼働中は不必要にスクリプトをいじらないことです。
 */

function privmsg(target, text)
{
	text = text.replace(/^\s+/, '').replace(/\s+$/, '');  // trim()
	var rawtext = 'PRIVMSG ' + target + ' :' + text;
	sendRaw(rawtext);
}

//-- [ 設定のようなもの ] ------------------------------------------------------

var targetChannel = '#gochabotdebug';   // 通知対象となるチャンネル (不要なら空白文字列)
var checkInterval = 1;                  // 監視を行う間隔(分)
var targetURLs = [
	{ name: '満席っぽいもの', id: 'A0025', url: 'http://rikunabi2010.yahoo.co.jp/bin/KDBG03200.cgi?KOKYAKU_ID=0084239001&FORM_ID=C002&MAGIC=BvnMScCoeoW1OwAA' },
	{ name: '予約できそうなの', id: 'SP012', url: 'http://rikunabi2010.yahoo.co.jp/bin/KDBG03200.cgi?KOKYAKU_ID=0102997001&FORM_ID=C002&MAGIC=RfnMScCoeZ2_aQAA' },
	{ name: '未対応形式ページ', id: 'ERROR', url: 'http://www.google.co.jp/' }
];  // 監視対象となるURLと、日程表に書かれているIDの組 (最後のアイテムには , をつけない)

// 予約状況が満席でないと思しきときの処理
function OnNotReserved() {
	var msg = "「"+targetURLs[targetIndex]["name"]+"」" + "は現在予約できる可能性があります！";

	// スクリプトコンソールへの表示
	log(msg + "\n" + targetURLs[targetIndex]["url"]);
	// バルーンによる通知
	showBalloon("予約状況の通知", msg);
	// 指定チャンネルへの通知
	if (targetChannel != '')
		send(targetChannel, "\x02\x0303【通知】\x02\x0301" + msg + "\x0f " + targetURLs[targetIndex]["url"]);
	// ブラウザで予約ページを開く
	// [非推奨！] 気づかず放置するとページを開き続けて大変なことに
	//browseUrl(targetURLs[targetIndex]["url"]);
}

// 予約状況が満席のときの処理
function OnReserved() {
	var msg = "「"+targetURLs[targetIndex]["name"]+"」" + "の予約状況は満席のようです";

	// スクリプトコンソールへの表示
	log(msg);
	// 指定チャンネルへの通知
	//if (targetChannel != '')
	//	send(targetChannel, "" + msg /*+ " " + targetURLs[targetIndex]["url"]*/);
	// バルーンによる通知
	//showBalloon("予約状況の通知", msg);
}

// 予約状況確認以前にエラーが出たときの処理
function OnError(msg) {
	// スクリプトコンソールへの表示
	log('エラー: ' + msg);
	// 指定チャンネルへの通知
	if (targetChannel != '')
		send(targetChannel, "\x02\x0304【エラー】\x02\x0301" + msg + "\x0f " + targetURLs[targetIndex]["url"]);
	// バルーンによる通知
	showBalloon("予約状況の確認エラー", msg);
}

//-- [ global / initialize ] ---------------------------------------------------

var rikunabiCheckLocked = false;
var targetIndex = 0;

function randomInt2(a, b) {
	return Math.floor(Math.random() * (Math.abs(b-a)+1)) + Math.min(a,b);
}

function event::onLoad()
{
	targetIndex = randomInt2(0, targetURLs.length - 1);
	if (checkInterval > 0)
		setInterval(timerRoutine, checkInterval * 60 * 1000);
}

//-- [ common functions ] ------------------------------------------------------

function trim(str)
{
	return str.replace(/^\s+/, '').replace(/\s+$/, '');
}

function stripTextDecoration(str)
{
	// \x02: <bold> \x03aa,bb: <color aa,bb>
	// \x0f: <stop> \x16: <reverse> \x1f: <underline>
	return str.replace(/[\x02\x0f\x16\x1f]|\x03\d\d(?:,\d\d)?/g, '');
}

var entity_list = {"ne":8800,"le":8804,"para":182,"xi":958,"darr":8595,"nu":957,"oacute":243,"Uacute":218,"omega":969,"prime":8242,"pound":163,"igrave":236,"thorn":254,"forall":8704,"emsp":8195,"lowast":8727,"brvbar":166,"alefsym":8501,"nbsp":160,"delta":948,"clubs":9827,"lArr":8656,"Omega":937,"quot":34,"Auml":196,"cedil":184,"and":8743,"plusmn":177,"ge":8805,"raquo":187,"uml":168,"equiv":8801,"laquo":171,"Epsilon":917,"rdquo":8221,"divide":247,"fnof":402,"chi":967,"Dagger":8225,"iacute":237,"rceil":8969,"sigma":963,"Oslash":216,"acute":180,"frac34":190,"upsih":978,"lrm":8206,"Scaron":352,"part":8706,"exist":8707,"nabla":8711,"image":8465,"prop":8733,"omicron":959,"zwj":8205,"gt":62,"aacute":225,"Yuml":376,"Yacute":221,"weierp":8472,"rsquo":8217,"otimes":8855,"kappa":954,"thetasym":977,"harr":8596,"Ouml":214,"Iota":921,"ograve":242,"sdot":8901,"copy":169,"oplus":8853,"acirc":226,"sup":8835,"zeta":950,"Iacute":205,"Oacute":211,"crarr":8629,"Nu":925,"bdquo":8222,"lsquo":8216,"Beta":914,"eacute":233,"egrave":232,"lceil":8968,"Kappa":922,"piv":982,"Ccedil":199,"ldquo":8220,"Xi":926,"cent":162,"uarr":8593,"hellip":8230,"Aacute":193,"ensp":8194,"sect":167,"Ugrave":217,"aelig":230,"ordf":170,"curren":164,"sbquo":8218,"macr":175,"Phi":934,"Eta":919,"rho":961,"Omicron":927,"sup2":178,"euro":8364,"aring":229,"Theta":920,"mdash":8212,"uuml":252,"otilde":245,"eta":951,"uacute":250,"rArr":8658,"nsub":8836,"agrave":224,"notin":8713,"Psi":936,"ndash":8211,"Ocirc":212,"sube":8838,"szlig":223,"micro":181,"not":172,"sup1":185,"middot":183,"iota":953,"ecirc":234,"lsaquo":8249,"thinsp":8201,"sum":8721,"ntilde":241,"scaron":353,"cap":8745,"atilde":227,"lang":9001,"isin":8712,"gamma":947,"Euml":203,"ang":8736,"upsilon":965,"Ntilde":209,"hearts":9829,"Tau":932,"Alpha":913,"spades":9824,"THORN":222,"dagger":8224,"int":8747,"lambda":955,"Eacute":201,"Uuml":220,"infin":8734,"Aring":197,"rlm":8207,"ugrave":249,"Egrave":200,"Acirc":194,"ETH":208,"oslash":248,"rsaquo":8250,"alpha":945,"Ograve":210,"Prime":8243,"mu":956,"ni":8715,"real":8476,"bull":8226,"beta":946,"icirc":238,"eth":240,"prod":8719,"larr":8592,"ordm":186,"perp":8869,"Gamma":915,"Pi":928,"reg":174,"ucirc":251,"psi":968,"tilde":732,"asymp":8776,"zwnj":8204,"Agrave":192,"Delta":916,"deg":176,"AElig":198,"times":215,"sim":8764,"Mu":924,"Otilde":213,"uArr":8657,"circ":710,"theta":952,"Rho":929,"sup3":179,"diams":9830,"tau":964,"Chi":935,"frac14":188,"oelig":339,"shy":173,"or":8744,"dArr":8659,"phi":966,"Lambda":923,"iuml":239,"rfloor":8971,"iexcl":161,"cong":8773,"ccedil":231,"Icirc":206,"frac12":189,"loz":9674,"rarr":8594,"cup":8746,"radic":8730,"frasl":8260,"euml":235,"OElig":338,"hArr":8660,"Atilde":195,"lt":60,"Upsilon":933,"there4":8756,"ouml":246,"oline":8254,"Ecirc":202,"yacute":253,"amp":38,"auml":228,"sigmaf":962,"permil":8240,"iquest":191,"empty":8709,"pi":960,"Ucirc":219,"supe":8839,"Igrave":204,"yen":165,"rang":9002,"trade":8482,"lfloor":8970,"minus":8722,"Zeta":918,"sub":8834,"epsilon":949,"Sigma":931,"yuml":255,"Iuml":207,"ocirc":244};
function decodeEntities(str)
{
	return str.replace(/&#(\d+);?|&#x([0-9a-fA-F]+);?|&(AElig|Aacute|Acirc|Agrave|Alpha|Aring|Atilde|Auml|Beta|Ccedil|Chi|Dagger|Delta|ETH|Eacute|Ecirc|Egrave|Epsilon|Eta|Euml|Gamma|Iacute|Icirc|Igrave|Iota|Iuml|Kappa|Lambda|Mu|Ntilde|Nu|OElig|Oacute|Ocirc|Ograve|Omega|Omicron|Oslash|Otilde|Ouml|Phi|Pi|Prime|Psi|Rho|Scaron|Sigma|THORN|Tau|Theta|Uacute|Ucirc|Ugrave|Upsilon|Uuml|Xi|Yacute|Yuml|Zeta|aacute|acirc|acute|aelig|agrave|alefsym|alpha|amp|and|ang|aring|asymp|atilde|auml|bdquo|beta|brvbar|bull|cap|ccedil|cedil|cent|chi|circ|clubs|cong|copy|crarr|cup|curren|dArr|dagger|darr|deg|delta|diams|divide|eacute|ecirc|egrave|empty|emsp|ensp|epsilon|equiv|eta|eth|euml|euro|exist|fnof|forall|frac12|frac14|frac34|frasl|gamma|ge|gt|hArr|harr|hearts|hellip|iacute|icirc|iexcl|igrave|image|infin|int|iota|iquest|isin|iuml|kappa|lArr|lambda|lang|laquo|larr|lceil|ldquo|le|lfloor|lowast|loz|lrm|lsaquo|lsquo|lt|macr|mdash|micro|middot|minus|mu|nabla|nbsp|ndash|ne|ni|not|notin|nsub|ntilde|nu|oacute|ocirc|oelig|ograve|oline|omega|omicron|oplus|or|ordf|ordm|oslash|otilde|otimes|ouml|para|part|permil|perp|phi|pi|piv|plusmn|pound|prime|prod|prop|psi|quot|rArr|radic|rang|raquo|rarr|rceil|rdquo|real|reg|rfloor|rho|rlm|rsaquo|rsquo|sbquo|scaron|sdot|sect|shy|sigma|sigmaf|sim|spades|sub|sube|sum|sup|sup1|sup2|sup3|supe|szlig|tau|there4|theta|thetasym|thinsp|thorn|tilde|times|trade|uArr|uacute|uarr|ucirc|ugrave|uml|upsih|upsilon|uuml|weierp|xi|yacute|yen|yuml|zeta|zwj|zwnj);?/g,function(str, int, hex, ent){
		if (int) {
			return String.fromCharCode(int);
		} else if (hex) {
			return String.fromCharCode(parseInt("0x"+hex));
		} else if (entity_list[ent]) {
			return String.fromCharCode(entity_list[ent]);
		} else {
			return "&" + ent;
		}
	});
}

function XMLHttpRequest()
{
	try {
		return new ActiveXObject("Msxml2.ServerXMLHTTP.6.0");
	} catch(e) {}
	try {
		return new ActiveXObject("Msxml2.ServerXMLHTTP.5.0");
	} catch(e) {}
	try {
		return new ActiveXObject("Msxml2.ServerXMLHTTP.4.0");
	} catch(e) {}
	try {
		return new ActiveXObject("Msxml2.ServerXMLHTTP.3.0");
	} catch(e) {}
	try {
		return new ActiveXObject("Msxml2.ServerXMLHTTP");
	} catch(e) {}
}

//-- [ subroutine ] ------------------------------------------------------------

function checkRikunabi()
{
	var url = targetURLs[targetIndex]["url"];

	if (rikunabiCheckLocked)
		return;

	function processHtmlContent(content) {
		// 空白文字類の簡素化
		content = content.replace(/[ \t\r\n]+/g, " ");

		/*
		  判定対象項目のサンプルデータ:

			<table border="0" cellpadding="0" cellspacing="0" class="gh_cpy_tbl_evt02">
				<tr>
				<td class="gh_evt_btn"><input name="YOYAKU_CD" type="radio" value="0331A"></td>
				<td class="gh_evt_code">0331A</td>
				<td>
					<table border="0" cellpadding="0" cellspacing="0" class="gh_cpy_tbl_evt03">
						<tr>
							<th class="gh_evt_col02">2009/03/31（火）　10：00～</th>
							<th class="gh_evt_col03 g_txt_C">東京都</th>
							<th class="gh_evt_col04 g_txt_C"><a href="/bin/KDBG05200.cgi?MAGIC=jO3MScCoegHgdQAA&KOKYAKU_ID=0051283001&MAP_CD=0001" target="_blank">地図</a></th>
							<th class="gh_evt_col05 g_txt_C">満席</th>
						</tr>
						<tr>
							<td colspan="4">キャビン本社８F会議室</td>
						</tr>
					</table>

				</td>
			</tr>
		
		  本スクリプトではこの一部をごく曖昧にマッチングを試み、状態を判定する。
		 */

		var reRikunabiParser = new RegExp('<td class="gh_evt_code">'+targetURLs[targetIndex]["id"]+'</td> ?<td> ?<table .*?<tr>.*?<th class="gh_evt_col05 g_txt_C">(.*?)</th> ?</tr>','i');
		if (content.match(reRikunabiParser)) {
			var deadline = RegExp.$1;
			if (deadline == "満席")
				OnReserved();
			else
				OnNotReserved();
		}
		else if (content.match(/<td class="gh_evt_col01">選択<\/td> ?<td class="gh_evt_col02">開催日時<\/td> ?<td class="gh_evt_col03">開催地<\/td> ?<td class="gh_evt_col04">地図<\/td> ?<td class="gh_evt_col05">受付締切<\/td>/)) {
			OnError("「"+targetURLs[targetIndex]["name"]+"」" + "指定のイベントが見つかりません。");
		}
		else {
			OnError("「"+targetURLs[targetIndex]["name"]+"」" + "表の解析に失敗しました。未対応形式のページを指定している可能性があります。");
		}
		targetIndex = (targetIndex + 1) % targetURLs.length;
	}

	var maxLength = 32768;

	var xmlhttp = XMLHttpRequest();
	if (!xmlhttp) {
		OnError('XMLHttpRequest() failed');
	}

	xmlhttp.onreadystatechange = function() {
		switch (xmlhttp.readyState) {
		case 1: // Open
		case 2: // Sent
		case 3: // Receiving
			break;
		case 4: // Loaded
			var charset = '';

			// obtain proper charset first
			var contentType = xmlhttp.getResponseHeader("Content-Type");
			var contentLength = xmlhttp.getResponseHeader('Content-Length');
			var contentRange = xmlhttp.getResponseHeader('Content-Range');
			contentType.match(/([^;]+);?/);
			var mime = RegExp.$1;
			var isHTML = mime.match(/xml|html/i);
			var getCharsetHTTP = new RegExp('charset=([^ \t\n;]+)',"i");
			var getCharsetXML = new RegExp('<\?xml[^>]+encoding="([^"]+)"',"i");
			var getMetaCharsetHTML = new RegExp('(<meta[^>]*?http-equiv="?content-type"?[^>]*>)',"i");
			var getCharsetHTMLFromMeta = new RegExp('charset="?([^ \t\n"]+)',"i");
			if (isHTML && xmlhttp.responseText.match(getCharsetXML))
				charset = RegExp.$1;
			else if (isHTML && xmlhttp.responseText.match(getMetaCharsetHTML)) {
				var metaContentType = RegExp.$1;
				if (metaContentType.match(getCharsetHTMLFromMeta))
					charset = RegExp.$1;
			}
			else if (contentType.match(getCharsetHTTP)) {
				var getCharsetHTML = new RegExp('<meta[ \t\n]+http-equiv="?content-type"?',"i");
				charset = RegExp.$1;
			}

			var stream = new ActiveXObject("ADODB.Stream");
			stream.Open();
			stream.Type = 1;
			stream.Write(xmlhttp.responseBody);
			stream.Position = 0;
			stream.Type = 2;
			stream.Charset = (charset != '') ? charset : '_autodetect';
			content = stream.ReadText();
			processHtmlContent(content);
			break;
		}
	};
	xmlhttp.setTimeouts(5*1000, 5*1000, 15*1000, 15*1000);
	xmlhttp.open('GET', url, true);
	if (maxLength > 0)
		xmlhttp.setRequestHeader('Range', 'bytes=0-' + maxLength);
	xmlhttp.setRequestHeader('User-Agent', 'Mozilla/5.0 (compatible; rikunabi-browser)');
	xmlhttp.send('');
}

//-- [ entry point ] -----------------------------------------------------------

function onTextPost(prefix, target, text)
{
	text = stripTextDecoration(text);

	if (prefix.nick != myNick)
		return;

	if (text.match(/^\.rikunabi$/)) {
		checkRikunabi();
	}
}

function timerRoutine() {
	checkRikunabi();
}

function event::onChannelText(prefix, channel, text)
{
	onTextPost(prefix, channel, text);
}

function event::onTalkText(prefix, targetNick, text)
{
	if (targetNick == myNick)
		onTextPost(prefix, prefix.nick, text);
	else
		onTextPost(prefix, targetNick, text);
}
