using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace morsegen
{
	/// <summary>
	/// Form1 の概要の説明です。
	/// </summary>
	public class morsegen : System.Windows.Forms.Form
	{
		private System.Windows.Forms.SaveFileDialog SaveFileDialog;
		private System.Windows.Forms.Label MsgTextLabel;
		private System.Windows.Forms.TextBox MsgTextBox;
		private System.Windows.Forms.Label TempoLabel;
		private System.Windows.Forms.TextBox WpmTextBox;
		private System.Windows.Forms.Label WpmLabel;
		private System.Windows.Forms.Label NofityLabel;
		private System.Windows.Forms.CheckBox TitleIsMsgCheckBox;
		private System.Windows.Forms.CheckBox CompressCheckBox;
		private System.Windows.Forms.Button SaveButton;
		private System.Windows.Forms.GroupBox MelodyGroupBox;
		private System.Windows.Forms.Label ResetTypeLabel;
		private System.Windows.Forms.Label NoteNumberLabel;
		private System.Windows.Forms.TrackBar NoteNumberTrackBar;
		private System.Windows.Forms.Label NoteNumberValueLabel;
		private System.Windows.Forms.Label VelocityLabel;
		private System.Windows.Forms.TrackBar VelocityTrackBar;
		private System.Windows.Forms.Label VelocityValueLabel;
		private System.Windows.Forms.RadioButton GM1ResetRadioButton;
		private System.Windows.Forms.RadioButton GM2ResetRadioButton;
		private System.Windows.Forms.CheckBox GSResetCheckBox;
		private System.Windows.Forms.CheckBox XGResetCheckBox;
		private System.Windows.Forms.Label PatchLabel;
		private System.Windows.Forms.Label BankMsbLabel;
		private System.Windows.Forms.TextBox BankMsbTextBox;
		private System.Windows.Forms.Label BankLsbLabel;
		private System.Windows.Forms.TextBox BankLsbTextBox;
		private System.Windows.Forms.Label PatchNumberLabel;
		private System.Windows.Forms.TextBox PatchNumberTextBox;
		private System.Windows.Forms.Label VolumeLabel;
		private System.Windows.Forms.TrackBar VolumeTrackBar;
		private System.Windows.Forms.Label VolumeValueLabel;
		private System.Windows.Forms.Label PanpotLabel;
		private System.Windows.Forms.CheckBox RndPanCheckBox;
		private System.Windows.Forms.Label ReverbLabel;
		private System.Windows.Forms.TrackBar ReverbTrackBar;
		private System.Windows.Forms.Label ReverbValueLabel;
		private System.Windows.Forms.Label ChorusLabel;
		private System.Windows.Forms.TrackBar ChorusTrackBar;
		private System.Windows.Forms.Label ChorusValueLabel;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public morsegen()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(morsegen));
			this.SaveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.MsgTextLabel = new System.Windows.Forms.Label();
			this.MsgTextBox = new System.Windows.Forms.TextBox();
			this.TempoLabel = new System.Windows.Forms.Label();
			this.WpmTextBox = new System.Windows.Forms.TextBox();
			this.WpmLabel = new System.Windows.Forms.Label();
			this.NofityLabel = new System.Windows.Forms.Label();
			this.TitleIsMsgCheckBox = new System.Windows.Forms.CheckBox();
			this.CompressCheckBox = new System.Windows.Forms.CheckBox();
			this.SaveButton = new System.Windows.Forms.Button();
			this.MelodyGroupBox = new System.Windows.Forms.GroupBox();
			this.ResetTypeLabel = new System.Windows.Forms.Label();
			this.GM1ResetRadioButton = new System.Windows.Forms.RadioButton();
			this.GM2ResetRadioButton = new System.Windows.Forms.RadioButton();
			this.GSResetCheckBox = new System.Windows.Forms.CheckBox();
			this.XGResetCheckBox = new System.Windows.Forms.CheckBox();
			this.PatchLabel = new System.Windows.Forms.Label();
			this.BankMsbLabel = new System.Windows.Forms.Label();
			this.BankMsbTextBox = new System.Windows.Forms.TextBox();
			this.BankLsbLabel = new System.Windows.Forms.Label();
			this.BankLsbTextBox = new System.Windows.Forms.TextBox();
			this.PatchNumberLabel = new System.Windows.Forms.Label();
			this.PatchNumberTextBox = new System.Windows.Forms.TextBox();
			this.VolumeLabel = new System.Windows.Forms.Label();
			this.VolumeTrackBar = new System.Windows.Forms.TrackBar();
			this.VolumeValueLabel = new System.Windows.Forms.Label();
			this.PanpotLabel = new System.Windows.Forms.Label();
			this.RndPanCheckBox = new System.Windows.Forms.CheckBox();
			this.ReverbLabel = new System.Windows.Forms.Label();
			this.ReverbTrackBar = new System.Windows.Forms.TrackBar();
			this.ReverbValueLabel = new System.Windows.Forms.Label();
			this.ChorusLabel = new System.Windows.Forms.Label();
			this.ChorusTrackBar = new System.Windows.Forms.TrackBar();
			this.ChorusValueLabel = new System.Windows.Forms.Label();
			this.VelocityLabel = new System.Windows.Forms.Label();
			this.NoteNumberLabel = new System.Windows.Forms.Label();
			this.VelocityTrackBar = new System.Windows.Forms.TrackBar();
			this.NoteNumberTrackBar = new System.Windows.Forms.TrackBar();
			this.VelocityValueLabel = new System.Windows.Forms.Label();
			this.NoteNumberValueLabel = new System.Windows.Forms.Label();
			this.MelodyGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.VolumeTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.ReverbTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.ChorusTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.VelocityTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.NoteNumberTrackBar)).BeginInit();
			this.SuspendLayout();
			// 
			// SaveFileDialog
			// 
			this.SaveFileDialog.DefaultExt = "mid";
			this.SaveFileDialog.Filter = "標準MIDIファイル (*.mid)|*.mid|すべてのファイル (*.*)|*.*";
			// 
			// MsgTextLabel
			// 
			this.MsgTextLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.MsgTextLabel.Location = new System.Drawing.Point(8, 8);
			this.MsgTextLabel.Name = "MsgTextLabel";
			this.MsgTextLabel.Size = new System.Drawing.Size(104, 19);
			this.MsgTextLabel.TabIndex = 0;
			this.MsgTextLabel.Text = "メッセージ(&E)";
			this.MsgTextLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// MsgTextBox
			// 
			this.MsgTextBox.Location = new System.Drawing.Point(120, 8);
			this.MsgTextBox.Name = "MsgTextBox";
			this.MsgTextBox.Size = new System.Drawing.Size(344, 19);
			this.MsgTextBox.TabIndex = 0;
			this.MsgTextBox.Text = "";
			// 
			// TempoLabel
			// 
			this.TempoLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.TempoLabel.Location = new System.Drawing.Point(8, 32);
			this.TempoLabel.Name = "TempoLabel";
			this.TempoLabel.Size = new System.Drawing.Size(104, 19);
			this.TempoLabel.TabIndex = 1;
			this.TempoLabel.Text = "速度(&H)";
			this.TempoLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// WpmTextBox
			// 
			this.WpmTextBox.Location = new System.Drawing.Point(120, 32);
			this.WpmTextBox.MaxLength = 2;
			this.WpmTextBox.Name = "WpmTextBox";
			this.WpmTextBox.Size = new System.Drawing.Size(32, 19);
			this.WpmTextBox.TabIndex = 1;
			this.WpmTextBox.Text = "25";
			// 
			// WpmLabel
			// 
			this.WpmLabel.Location = new System.Drawing.Point(152, 32);
			this.WpmLabel.Name = "WpmLabel";
			this.WpmLabel.Size = new System.Drawing.Size(40, 19);
			this.WpmLabel.TabIndex = 7;
			this.WpmLabel.Text = "[WPM]";
			this.WpmLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// NofityLabel
			// 
			this.NofityLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.NofityLabel.ForeColor = System.Drawing.SystemColors.GrayText;
			this.NofityLabel.Location = new System.Drawing.Point(200, 32);
			this.NofityLabel.Name = "NofityLabel";
			this.NofityLabel.Size = new System.Drawing.Size(264, 19);
			this.NofityLabel.TabIndex = 2;
			this.NofityLabel.Text = "※一部文字を代替表現しています";
			this.NofityLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// TitleIsMsgCheckBox
			// 
			this.TitleIsMsgCheckBox.Checked = true;
			this.TitleIsMsgCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.TitleIsMsgCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.TitleIsMsgCheckBox.Location = new System.Drawing.Point(8, 56);
			this.TitleIsMsgCheckBox.Name = "TitleIsMsgCheckBox";
			this.TitleIsMsgCheckBox.Size = new System.Drawing.Size(184, 19);
			this.TitleIsMsgCheckBox.TabIndex = 3;
			this.TitleIsMsgCheckBox.Text = "曲名をメッセージ内容にする(&I)";
			// 
			// CompressCheckBox
			// 
			this.CompressCheckBox.Checked = true;
			this.CompressCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.CompressCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.CompressCheckBox.Location = new System.Drawing.Point(200, 56);
			this.CompressCheckBox.Name = "CompressCheckBox";
			this.CompressCheckBox.Size = new System.Drawing.Size(184, 19);
			this.CompressCheckBox.TabIndex = 4;
			this.CompressCheckBox.Text = "ランニングステータス圧縮有効(&R)";
			// 
			// SaveButton
			// 
			this.SaveButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.SaveButton.Location = new System.Drawing.Point(392, 56);
			this.SaveButton.Name = "SaveButton";
			this.SaveButton.Size = new System.Drawing.Size(72, 19);
			this.SaveButton.TabIndex = 5;
			this.SaveButton.Text = "保存(&S)";
			this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
			// 
			// MelodyGroupBox
			// 
			this.MelodyGroupBox.Controls.Add(this.ResetTypeLabel);
			this.MelodyGroupBox.Controls.Add(this.GM1ResetRadioButton);
			this.MelodyGroupBox.Controls.Add(this.GM2ResetRadioButton);
			this.MelodyGroupBox.Controls.Add(this.GSResetCheckBox);
			this.MelodyGroupBox.Controls.Add(this.XGResetCheckBox);
			this.MelodyGroupBox.Controls.Add(this.PatchLabel);
			this.MelodyGroupBox.Controls.Add(this.BankMsbLabel);
			this.MelodyGroupBox.Controls.Add(this.BankMsbTextBox);
			this.MelodyGroupBox.Controls.Add(this.BankLsbLabel);
			this.MelodyGroupBox.Controls.Add(this.BankLsbTextBox);
			this.MelodyGroupBox.Controls.Add(this.PatchNumberLabel);
			this.MelodyGroupBox.Controls.Add(this.PatchNumberTextBox);
			this.MelodyGroupBox.Controls.Add(this.VolumeLabel);
			this.MelodyGroupBox.Controls.Add(this.VolumeTrackBar);
			this.MelodyGroupBox.Controls.Add(this.VolumeValueLabel);
			this.MelodyGroupBox.Controls.Add(this.PanpotLabel);
			this.MelodyGroupBox.Controls.Add(this.RndPanCheckBox);
			this.MelodyGroupBox.Controls.Add(this.ReverbLabel);
			this.MelodyGroupBox.Controls.Add(this.ReverbTrackBar);
			this.MelodyGroupBox.Controls.Add(this.ReverbValueLabel);
			this.MelodyGroupBox.Controls.Add(this.ChorusLabel);
			this.MelodyGroupBox.Controls.Add(this.ChorusTrackBar);
			this.MelodyGroupBox.Controls.Add(this.ChorusValueLabel);
			this.MelodyGroupBox.Controls.Add(this.VelocityLabel);
			this.MelodyGroupBox.Controls.Add(this.NoteNumberLabel);
			this.MelodyGroupBox.Controls.Add(this.VelocityTrackBar);
			this.MelodyGroupBox.Controls.Add(this.NoteNumberTrackBar);
			this.MelodyGroupBox.Controls.Add(this.VelocityValueLabel);
			this.MelodyGroupBox.Controls.Add(this.NoteNumberValueLabel);
			this.MelodyGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.MelodyGroupBox.Location = new System.Drawing.Point(8, 88);
			this.MelodyGroupBox.Name = "MelodyGroupBox";
			this.MelodyGroupBox.Size = new System.Drawing.Size(456, 216);
			this.MelodyGroupBox.TabIndex = 6;
			this.MelodyGroupBox.TabStop = false;
			this.MelodyGroupBox.Text = "音色設定";
			// 
			// ResetTypeLabel
			// 
			this.ResetTypeLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ResetTypeLabel.Location = new System.Drawing.Point(8, 64);
			this.ResetTypeLabel.Name = "ResetTypeLabel";
			this.ResetTypeLabel.Size = new System.Drawing.Size(96, 19);
			this.ResetTypeLabel.TabIndex = 2;
			this.ResetTypeLabel.Text = "リセット";
			this.ResetTypeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// GM1ResetRadioButton
			// 
			this.GM1ResetRadioButton.Checked = true;
			this.GM1ResetRadioButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.GM1ResetRadioButton.Location = new System.Drawing.Point(112, 64);
			this.GM1ResetRadioButton.Name = "GM1ResetRadioButton";
			this.GM1ResetRadioButton.Size = new System.Drawing.Size(64, 19);
			this.GM1ResetRadioButton.TabIndex = 2;
			this.GM1ResetRadioButton.TabStop = true;
			this.GM1ResetRadioButton.Text = "GM1(&1)";
			// 
			// GM2ResetRadioButton
			// 
			this.GM2ResetRadioButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.GM2ResetRadioButton.Location = new System.Drawing.Point(192, 64);
			this.GM2ResetRadioButton.Name = "GM2ResetRadioButton";
			this.GM2ResetRadioButton.Size = new System.Drawing.Size(72, 19);
			this.GM2ResetRadioButton.TabIndex = 2;
			this.GM2ResetRadioButton.Text = "GM2(&2)";
			// 
			// GSResetCheckBox
			// 
			this.GSResetCheckBox.Checked = true;
			this.GSResetCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.GSResetCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.GSResetCheckBox.Location = new System.Drawing.Point(272, 64);
			this.GSResetCheckBox.Name = "GSResetCheckBox";
			this.GSResetCheckBox.Size = new System.Drawing.Size(72, 19);
			this.GSResetCheckBox.TabIndex = 3;
			this.GSResetCheckBox.Text = "GS(&G)";
			// 
			// XGResetCheckBox
			// 
			this.XGResetCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.XGResetCheckBox.Location = new System.Drawing.Point(352, 64);
			this.XGResetCheckBox.Name = "XGResetCheckBox";
			this.XGResetCheckBox.Size = new System.Drawing.Size(72, 19);
			this.XGResetCheckBox.TabIndex = 4;
			this.XGResetCheckBox.Text = "XG(&X)";
			// 
			// PatchLabel
			// 
			this.PatchLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.PatchLabel.Location = new System.Drawing.Point(8, 88);
			this.PatchLabel.Name = "PatchLabel";
			this.PatchLabel.Size = new System.Drawing.Size(96, 19);
			this.PatchLabel.TabIndex = 5;
			this.PatchLabel.Text = "音色";
			this.PatchLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// BankMsbLabel
			// 
			this.BankMsbLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.BankMsbLabel.Location = new System.Drawing.Point(112, 88);
			this.BankMsbLabel.Name = "BankMsbLabel";
			this.BankMsbLabel.Size = new System.Drawing.Size(64, 19);
			this.BankMsbLabel.TabIndex = 5;
			this.BankMsbLabel.Text = "MSB(&M)";
			this.BankMsbLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// BankMsbTextBox
			// 
			this.BankMsbTextBox.Location = new System.Drawing.Point(184, 88);
			this.BankMsbTextBox.MaxLength = 3;
			this.BankMsbTextBox.Name = "BankMsbTextBox";
			this.BankMsbTextBox.Size = new System.Drawing.Size(32, 19);
			this.BankMsbTextBox.TabIndex = 5;
			this.BankMsbTextBox.Text = "8";
			// 
			// BankLsbLabel
			// 
			this.BankLsbLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.BankLsbLabel.Location = new System.Drawing.Point(224, 88);
			this.BankLsbLabel.Name = "BankLsbLabel";
			this.BankLsbLabel.Size = new System.Drawing.Size(64, 19);
			this.BankLsbLabel.TabIndex = 6;
			this.BankLsbLabel.Text = "LSB(&L)";
			this.BankLsbLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// BankLsbTextBox
			// 
			this.BankLsbTextBox.Location = new System.Drawing.Point(296, 88);
			this.BankLsbTextBox.MaxLength = 3;
			this.BankLsbTextBox.Name = "BankLsbTextBox";
			this.BankLsbTextBox.Size = new System.Drawing.Size(32, 19);
			this.BankLsbTextBox.TabIndex = 6;
			this.BankLsbTextBox.Text = "0";
			// 
			// PatchNumberLabel
			// 
			this.PatchNumberLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.PatchNumberLabel.Location = new System.Drawing.Point(336, 88);
			this.PatchNumberLabel.Name = "PatchNumberLabel";
			this.PatchNumberLabel.Size = new System.Drawing.Size(64, 19);
			this.PatchNumberLabel.TabIndex = 7;
			this.PatchNumberLabel.Text = "Patch(&P)";
			this.PatchNumberLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// PatchNumberTextBox
			// 
			this.PatchNumberTextBox.Location = new System.Drawing.Point(408, 88);
			this.PatchNumberTextBox.MaxLength = 3;
			this.PatchNumberTextBox.Name = "PatchNumberTextBox";
			this.PatchNumberTextBox.Size = new System.Drawing.Size(40, 19);
			this.PatchNumberTextBox.TabIndex = 7;
			this.PatchNumberTextBox.Text = "81";
			// 
			// VolumeLabel
			// 
			this.VolumeLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.VolumeLabel.Location = new System.Drawing.Point(8, 112);
			this.VolumeLabel.Name = "VolumeLabel";
			this.VolumeLabel.Size = new System.Drawing.Size(96, 19);
			this.VolumeLabel.TabIndex = 8;
			this.VolumeLabel.Text = "音量(&V)";
			this.VolumeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// VolumeTrackBar
			// 
			this.VolumeTrackBar.AutoSize = false;
			this.VolumeTrackBar.LargeChange = 20;
			this.VolumeTrackBar.Location = new System.Drawing.Point(112, 112);
			this.VolumeTrackBar.Maximum = 127;
			this.VolumeTrackBar.Name = "VolumeTrackBar";
			this.VolumeTrackBar.Size = new System.Drawing.Size(288, 19);
			this.VolumeTrackBar.TabIndex = 8;
			this.VolumeTrackBar.TickFrequency = 16;
			this.VolumeTrackBar.Value = 100;
			this.VolumeTrackBar.ValueChanged += new System.EventHandler(this.VolumeTrackBar_ValueChanged);
			// 
			// VolumeValueLabel
			// 
			this.VolumeValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.VolumeValueLabel.Location = new System.Drawing.Point(408, 112);
			this.VolumeValueLabel.Name = "VolumeValueLabel";
			this.VolumeValueLabel.Size = new System.Drawing.Size(40, 19);
			this.VolumeValueLabel.TabIndex = 8;
			// 
			// PanpotLabel
			// 
			this.PanpotLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.PanpotLabel.Location = new System.Drawing.Point(8, 136);
			this.PanpotLabel.Name = "PanpotLabel";
			this.PanpotLabel.Size = new System.Drawing.Size(96, 19);
			this.PanpotLabel.TabIndex = 9;
			this.PanpotLabel.Text = "定位";
			this.PanpotLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// RndPanCheckBox
			// 
			this.RndPanCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.RndPanCheckBox.Location = new System.Drawing.Point(112, 136);
			this.RndPanCheckBox.Name = "RndPanCheckBox";
			this.RndPanCheckBox.Size = new System.Drawing.Size(232, 19);
			this.RndPanCheckBox.TabIndex = 9;
			this.RndPanCheckBox.Text = "符号ごとに定位をランダムに変更する(&A)";
			// 
			// ReverbLabel
			// 
			this.ReverbLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ReverbLabel.Location = new System.Drawing.Point(8, 160);
			this.ReverbLabel.Name = "ReverbLabel";
			this.ReverbLabel.Size = new System.Drawing.Size(96, 19);
			this.ReverbLabel.TabIndex = 10;
			this.ReverbLabel.Text = "リバーブ(&B)";
			this.ReverbLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// ReverbTrackBar
			// 
			this.ReverbTrackBar.AutoSize = false;
			this.ReverbTrackBar.LargeChange = 20;
			this.ReverbTrackBar.Location = new System.Drawing.Point(112, 160);
			this.ReverbTrackBar.Maximum = 127;
			this.ReverbTrackBar.Name = "ReverbTrackBar";
			this.ReverbTrackBar.Size = new System.Drawing.Size(288, 19);
			this.ReverbTrackBar.TabIndex = 10;
			this.ReverbTrackBar.TickFrequency = 16;
			this.ReverbTrackBar.ValueChanged += new System.EventHandler(this.ReverbTrackBar_ValueChanged);
			// 
			// ReverbValueLabel
			// 
			this.ReverbValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ReverbValueLabel.Location = new System.Drawing.Point(408, 160);
			this.ReverbValueLabel.Name = "ReverbValueLabel";
			this.ReverbValueLabel.Size = new System.Drawing.Size(40, 19);
			this.ReverbValueLabel.TabIndex = 10;
			// 
			// ChorusLabel
			// 
			this.ChorusLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ChorusLabel.Location = new System.Drawing.Point(8, 184);
			this.ChorusLabel.Name = "ChorusLabel";
			this.ChorusLabel.Size = new System.Drawing.Size(96, 19);
			this.ChorusLabel.TabIndex = 11;
			this.ChorusLabel.Text = "コーラス(&C)";
			this.ChorusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// ChorusTrackBar
			// 
			this.ChorusTrackBar.AutoSize = false;
			this.ChorusTrackBar.LargeChange = 20;
			this.ChorusTrackBar.Location = new System.Drawing.Point(112, 184);
			this.ChorusTrackBar.Maximum = 127;
			this.ChorusTrackBar.Name = "ChorusTrackBar";
			this.ChorusTrackBar.Size = new System.Drawing.Size(288, 19);
			this.ChorusTrackBar.TabIndex = 11;
			this.ChorusTrackBar.TickFrequency = 16;
			this.ChorusTrackBar.ValueChanged += new System.EventHandler(this.ChorusTrackBar_ValueChanged);
			// 
			// ChorusValueLabel
			// 
			this.ChorusValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.ChorusValueLabel.Location = new System.Drawing.Point(408, 184);
			this.ChorusValueLabel.Name = "ChorusValueLabel";
			this.ChorusValueLabel.Size = new System.Drawing.Size(40, 19);
			this.ChorusValueLabel.TabIndex = 11;
			// 
			// VelocityLabel
			// 
			this.VelocityLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.VelocityLabel.Location = new System.Drawing.Point(8, 40);
			this.VelocityLabel.Name = "VelocityLabel";
			this.VelocityLabel.Size = new System.Drawing.Size(96, 19);
			this.VelocityLabel.TabIndex = 1;
			this.VelocityLabel.Text = "ベロシティ(&O)";
			this.VelocityLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// NoteNumberLabel
			// 
			this.NoteNumberLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.NoteNumberLabel.Location = new System.Drawing.Point(8, 16);
			this.NoteNumberLabel.Name = "NoteNumberLabel";
			this.NoteNumberLabel.Size = new System.Drawing.Size(96, 19);
			this.NoteNumberLabel.TabIndex = 0;
			this.NoteNumberLabel.Text = "音程(&N)";
			this.NoteNumberLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// VelocityTrackBar
			// 
			this.VelocityTrackBar.AutoSize = false;
			this.VelocityTrackBar.LargeChange = 20;
			this.VelocityTrackBar.Location = new System.Drawing.Point(112, 40);
			this.VelocityTrackBar.Maximum = 127;
			this.VelocityTrackBar.Minimum = 1;
			this.VelocityTrackBar.Name = "VelocityTrackBar";
			this.VelocityTrackBar.Size = new System.Drawing.Size(288, 19);
			this.VelocityTrackBar.TabIndex = 1;
			this.VelocityTrackBar.TickFrequency = 16;
			this.VelocityTrackBar.Value = 100;
			this.VelocityTrackBar.ValueChanged += new System.EventHandler(this.VelocityTrackBar_ValueChanged);
			// 
			// NoteNumberTrackBar
			// 
			this.NoteNumberTrackBar.AutoSize = false;
			this.NoteNumberTrackBar.LargeChange = 20;
			this.NoteNumberTrackBar.Location = new System.Drawing.Point(112, 16);
			this.NoteNumberTrackBar.Maximum = 127;
			this.NoteNumberTrackBar.Name = "NoteNumberTrackBar";
			this.NoteNumberTrackBar.Size = new System.Drawing.Size(288, 19);
			this.NoteNumberTrackBar.TabIndex = 0;
			this.NoteNumberTrackBar.TickFrequency = 16;
			this.NoteNumberTrackBar.Value = 79;
			this.NoteNumberTrackBar.ValueChanged += new System.EventHandler(this.NoteNumberTrackBar_ValueChanged);
			// 
			// VelocityValueLabel
			// 
			this.VelocityValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.VelocityValueLabel.Location = new System.Drawing.Point(408, 40);
			this.VelocityValueLabel.Name = "VelocityValueLabel";
			this.VelocityValueLabel.Size = new System.Drawing.Size(40, 19);
			this.VelocityValueLabel.TabIndex = 1;
			// 
			// NoteNumberValueLabel
			// 
			this.NoteNumberValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.NoteNumberValueLabel.Location = new System.Drawing.Point(408, 16);
			this.NoteNumberValueLabel.Name = "NoteNumberValueLabel";
			this.NoteNumberValueLabel.Size = new System.Drawing.Size(40, 19);
			this.NoteNumberValueLabel.TabIndex = 0;
			// 
			// morsegen
			// 
			this.AcceptButton = this.SaveButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(470, 312);
			this.Controls.Add(this.WpmLabel);
			this.Controls.Add(this.WpmTextBox);
			this.Controls.Add(this.MsgTextLabel);
			this.Controls.Add(this.MsgTextBox);
			this.Controls.Add(this.TempoLabel);
			this.Controls.Add(this.NofityLabel);
			this.Controls.Add(this.TitleIsMsgCheckBox);
			this.Controls.Add(this.CompressCheckBox);
			this.Controls.Add(this.SaveButton);
			this.Controls.Add(this.MelodyGroupBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "morsegen";
			this.Text = "morsegen";
			this.Load += new System.EventHandler(this.morsegen_Load);
			this.MelodyGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.VolumeTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.ReverbTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.ChorusTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.VelocityTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.NoteNumberTrackBar)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() 
		{
            Application.EnableVisualStyles();
			Application.Run(new morsegen());
		}

		#region Initialize MorseTable
		/// <summary>
		/// モールス信号の変換テーブルを初期化します。
		/// </summary>
		private Hashtable MorseTable = new Hashtable();
		private void MorseTable_Init()
		{
			// 欧文
			MorseTable['a'] = "._";
			MorseTable['b'] = "_...";
			MorseTable['c'] = "_._.";
			MorseTable['d'] = "_..";
			MorseTable['e'] = ".";
			MorseTable['f'] = ".._.";
			MorseTable['g'] = "__.";
			MorseTable['h'] = "....";
			MorseTable['i'] = "..";
			MorseTable['j'] = ".___";
			MorseTable['k'] = "_._";
			MorseTable['l'] = "._..";
			MorseTable['m'] = "__";
			MorseTable['n'] = "_.";
			MorseTable['o'] = "___";
			MorseTable['p'] = ".__.";
			MorseTable['q'] = "__._";
			MorseTable['r'] = "._.";
			MorseTable['s'] = "...";
			MorseTable['t'] = "_";
			MorseTable['u'] = ".._";
			MorseTable['v'] = "..._";
			MorseTable['w'] = ".__";
			MorseTable['x'] = "_.._";
			MorseTable['y'] = "_.__";
			MorseTable['z'] = "__..";
			MorseTable['.'] = "._._._";
			MorseTable[','] = "__..__";
			MorseTable[':'] = "___...";
			MorseTable['?'] = "..__..";
			MorseTable['\''] = ".____.";
			MorseTable['-'] = "_...._";
			MorseTable['('] = "_.__.";
			MorseTable[')'] = "_.__._";
			MorseTable['/'] = "_.._.";
			MorseTable['='] = "_..._";
			MorseTable['+'] = "._._.";
			MorseTable['\"'] = "._.._.";
			MorseTable['*'] = "_.._";
			MorseTable['@'] = ".__._.";

			// 数字
			MorseTable['1'] = ".____";
			MorseTable['2'] = "..___";
			MorseTable['3'] = "...__";
			MorseTable['4'] = "...._";
			MorseTable['5'] = ".....";
			MorseTable['6'] = "_....";
			MorseTable['7'] = "__...";
			MorseTable['8'] = "___..";
			MorseTable['9'] = "____.";
			MorseTable['0'] = "_____";

			// 和文
			MorseTable['イ'] = "._";
			MorseTable['ロ'] = "._._";
			MorseTable['ハ'] = "_...";
			MorseTable['ニ'] = "_._.";
			MorseTable['ホ'] = "_..";
			MorseTable['ヘ'] = ".";
			MorseTable['ト'] = ".._..";
			MorseTable['チ'] = ".._.";
			MorseTable['リ'] = "__.";
			MorseTable['ヌ'] = "....";
			MorseTable['ル'] = "_.__.";
			MorseTable['ヲ'] = ".___";
			MorseTable['ワ'] = "_._";
			MorseTable['カ'] = "._..";
			MorseTable['ヨ'] = "__";
			MorseTable['タ'] = "_.";
			MorseTable['レ'] = "___";
			MorseTable['ソ'] = "___.";
			MorseTable['ツ'] = ".__.";
			MorseTable['ネ'] = "__._";
			MorseTable['ナ'] = "._.";
			MorseTable['ラ'] = "...";
			MorseTable['ム'] = "_";
			MorseTable['ウ'] = ".._";
			MorseTable['ヰ'] = "._.._";
			MorseTable['ノ'] = "..__";
			MorseTable['オ'] = "._...";
			MorseTable['ク'] = "..._";
			MorseTable['ヤ'] = ".__";
			MorseTable['マ'] = "_.._";
			MorseTable['ケ'] = "_.__";
			MorseTable['フ'] = "__..";
			MorseTable['コ'] = "____";
			MorseTable['エ'] = "_.___";
			MorseTable['テ'] = "._.__";
			MorseTable['ア'] = "__.__";
			MorseTable['サ'] = "_._._";
			MorseTable['キ'] = "_._..";
			MorseTable['ユ'] = "_..__";
			MorseTable['メ'] = "_..._";
			MorseTable['ミ'] = ".._._";
			MorseTable['シ'] = "__._.";
			MorseTable['ヱ'] = ".__..";
			MorseTable['ヒ'] = "__.._";
			MorseTable['モ'] = "_.._.";
			MorseTable['セ'] = ".___.";
			MorseTable['ス'] = "___._";
			MorseTable['ン'] = "._._.";
			MorseTable['゛'] = "..";
			MorseTable['゜'] = "..__.";
			MorseTable['ー'] = ".__._";
			MorseTable['、'] = "._._._";
			MorseTable['（'] = "_.__._";
			MorseTable['）'] = "._.._.";

			// 和文サロゲート合成など
			MorseTable['ヴ'] = MorseTable['ウ'] + " " + MorseTable['゛'];
			MorseTable['ガ'] = MorseTable['カ'] + " " + MorseTable['゛'];
			MorseTable['ギ'] = MorseTable['キ'] + " " + MorseTable['゛'];
			MorseTable['グ'] = MorseTable['ク'] + " " + MorseTable['゛'];
			MorseTable['ゲ'] = MorseTable['ケ'] + " " + MorseTable['゛'];
			MorseTable['ゴ'] = MorseTable['コ'] + " " + MorseTable['゛'];
			MorseTable['ザ'] = MorseTable['サ'] + " " + MorseTable['゛'];
			MorseTable['ジ'] = MorseTable['シ'] + " " + MorseTable['゛'];
			MorseTable['ズ'] = MorseTable['ス'] + " " + MorseTable['゛'];
			MorseTable['ゼ'] = MorseTable['セ'] + " " + MorseTable['゛'];
			MorseTable['ゾ'] = MorseTable['ソ'] + " " + MorseTable['゛'];
			MorseTable['ダ'] = MorseTable['タ'] + " " + MorseTable['゛'];
			MorseTable['ヂ'] = MorseTable['チ'] + " " + MorseTable['゛'];
			MorseTable['ヅ'] = MorseTable['ツ'] + " " + MorseTable['゛'];
			MorseTable['デ'] = MorseTable['テ'] + " " + MorseTable['゛'];
			MorseTable['ド'] = MorseTable['ト'] + " " + MorseTable['゛'];
			MorseTable['バ'] = MorseTable['ハ'] + " " + MorseTable['゛'];
			MorseTable['ビ'] = MorseTable['ヒ'] + " " + MorseTable['゛'];
			MorseTable['ブ'] = MorseTable['フ'] + " " + MorseTable['゛'];
			MorseTable['ベ'] = MorseTable['ヘ'] + " " + MorseTable['゛'];
			MorseTable['ボ'] = MorseTable['ホ'] + " " + MorseTable['゛'];
			MorseTable['パ'] = MorseTable['ハ'] + " " + MorseTable['゜'];
			MorseTable['ピ'] = MorseTable['ヒ'] + " " + MorseTable['゜'];
			MorseTable['プ'] = MorseTable['フ'] + " " + MorseTable['゜'];
			MorseTable['ペ'] = MorseTable['ヘ'] + " " + MorseTable['゜'];
			MorseTable['ポ'] = MorseTable['ホ'] + " " + MorseTable['゜'];
			MorseTable['ァ'] = MorseTable['ア'];
			MorseTable['ィ'] = MorseTable['イ'];
			MorseTable['ゥ'] = MorseTable['ウ'];
			MorseTable['ェ'] = MorseTable['エ'];
			MorseTable['ォ'] = MorseTable['オ'];
			MorseTable['ャ'] = MorseTable['ヤ'];
			MorseTable['ュ'] = MorseTable['ユ'];
			MorseTable['ョ'] = MorseTable['ヨ'];
			MorseTable['ヮ'] = MorseTable['ワ'];
			MorseTable['ヵ'] = MorseTable['カ'];
			MorseTable['ヶ'] = MorseTable['ケ'];
			MorseTable['ッ'] = MorseTable['ツ'];

			// 和文文字変換
			MorseTable['い'] = MorseTable['イ'];
			MorseTable['ろ'] = MorseTable['ロ'];
			MorseTable['は'] = MorseTable['ハ'];
			MorseTable['に'] = MorseTable['ニ'];
			MorseTable['ほ'] = MorseTable['ホ'];
			MorseTable['へ'] = MorseTable['ヘ'];
			MorseTable['と'] = MorseTable['ト'];
			MorseTable['ち'] = MorseTable['チ'];
			MorseTable['り'] = MorseTable['リ'];
			MorseTable['ぬ'] = MorseTable['ヌ'];
			MorseTable['る'] = MorseTable['ル'];
			MorseTable['を'] = MorseTable['ヲ'];
			MorseTable['わ'] = MorseTable['ワ'];
			MorseTable['か'] = MorseTable['カ'];
			MorseTable['よ'] = MorseTable['ヨ'];
			MorseTable['た'] = MorseTable['タ'];
			MorseTable['れ'] = MorseTable['レ'];
			MorseTable['そ'] = MorseTable['ソ'];
			MorseTable['つ'] = MorseTable['ツ'];
			MorseTable['ね'] = MorseTable['ネ'];
			MorseTable['な'] = MorseTable['ナ'];
			MorseTable['ら'] = MorseTable['ラ'];
			MorseTable['む'] = MorseTable['ム'];
			MorseTable['う'] = MorseTable['ウ'];
			MorseTable['ゐ'] = MorseTable['ヰ'];
			MorseTable['の'] = MorseTable['ノ'];
			MorseTable['お'] = MorseTable['オ'];
			MorseTable['く'] = MorseTable['ク'];
			MorseTable['や'] = MorseTable['ヤ'];
			MorseTable['ま'] = MorseTable['マ'];
			MorseTable['け'] = MorseTable['ケ'];
			MorseTable['ふ'] = MorseTable['フ'];
			MorseTable['こ'] = MorseTable['コ'];
			MorseTable['え'] = MorseTable['エ'];
			MorseTable['て'] = MorseTable['テ'];
			MorseTable['あ'] = MorseTable['ア'];
			MorseTable['さ'] = MorseTable['サ'];
			MorseTable['き'] = MorseTable['キ'];
			MorseTable['ゆ'] = MorseTable['ユ'];
			MorseTable['め'] = MorseTable['メ'];
			MorseTable['み'] = MorseTable['ミ'];
			MorseTable['し'] = MorseTable['シ'];
			MorseTable['ゑ'] = MorseTable['ヱ'];
			MorseTable['ひ'] = MorseTable['ヒ'];
			MorseTable['も'] = MorseTable['モ'];
			MorseTable['せ'] = MorseTable['セ'];
			MorseTable['す'] = MorseTable['ス'];
			MorseTable['ん'] = MorseTable['ン'];
			MorseTable['が'] = MorseTable['ガ'];
			MorseTable['ぎ'] = MorseTable['ギ'];
			MorseTable['ぐ'] = MorseTable['グ'];
			MorseTable['げ'] = MorseTable['ゲ'];
			MorseTable['ご'] = MorseTable['ゴ'];
			MorseTable['ざ'] = MorseTable['ザ'];
			MorseTable['じ'] = MorseTable['ジ'];
			MorseTable['ず'] = MorseTable['ズ'];
			MorseTable['ぜ'] = MorseTable['ゼ'];
			MorseTable['ぞ'] = MorseTable['ゾ'];
			MorseTable['だ'] = MorseTable['ダ'];
			MorseTable['ぢ'] = MorseTable['ヂ'];
			MorseTable['づ'] = MorseTable['ヅ'];
			MorseTable['で'] = MorseTable['デ'];
			MorseTable['ど'] = MorseTable['ド'];
			MorseTable['ば'] = MorseTable['バ'];
			MorseTable['び'] = MorseTable['ビ'];
			MorseTable['ぶ'] = MorseTable['ブ'];
			MorseTable['べ'] = MorseTable['ベ'];
			MorseTable['ぼ'] = MorseTable['ボ'];
			MorseTable['ぱ'] = MorseTable['パ'];
			MorseTable['ぴ'] = MorseTable['ピ'];
			MorseTable['ぷ'] = MorseTable['プ'];
			MorseTable['ぺ'] = MorseTable['ペ'];
			MorseTable['ぽ'] = MorseTable['ポ'];
			MorseTable['ぁ'] = MorseTable['ァ'];
			MorseTable['ぃ'] = MorseTable['ィ'];
			MorseTable['ぅ'] = MorseTable['ゥ'];
			MorseTable['ぇ'] = MorseTable['ェ'];
			MorseTable['ぉ'] = MorseTable['ォ'];
			MorseTable['ゃ'] = MorseTable['ャ'];
			MorseTable['ゅ'] = MorseTable['ュ'];
			MorseTable['ょ'] = MorseTable['ョ'];
			MorseTable['ゎ'] = MorseTable['ヮ'];
			MorseTable['っ'] = MorseTable['ッ'];

			// 欧文全角
			MorseTable['ａ'] = MorseTable['a'];
			MorseTable['ｂ'] = MorseTable['b'];
			MorseTable['ｃ'] = MorseTable['c'];
			MorseTable['ｄ'] = MorseTable['d'];
			MorseTable['ｅ'] = MorseTable['e'];
			MorseTable['ｆ'] = MorseTable['f'];
			MorseTable['ｇ'] = MorseTable['g'];
			MorseTable['ｈ'] = MorseTable['h'];
			MorseTable['ｉ'] = MorseTable['i'];
			MorseTable['ｊ'] = MorseTable['j'];
			MorseTable['ｋ'] = MorseTable['k'];
			MorseTable['ｌ'] = MorseTable['l'];
			MorseTable['ｍ'] = MorseTable['m'];
			MorseTable['ｎ'] = MorseTable['n'];
			MorseTable['ｏ'] = MorseTable['o'];
			MorseTable['ｐ'] = MorseTable['p'];
			MorseTable['ｑ'] = MorseTable['q'];
			MorseTable['ｒ'] = MorseTable['r'];
			MorseTable['ｓ'] = MorseTable['s'];
			MorseTable['ｔ'] = MorseTable['t'];
			MorseTable['ｕ'] = MorseTable['u'];
			MorseTable['ｖ'] = MorseTable['v'];
			MorseTable['ｗ'] = MorseTable['w'];
			MorseTable['ｘ'] = MorseTable['x'];
			MorseTable['ｙ'] = MorseTable['y'];
			MorseTable['ｚ'] = MorseTable['z'];
			MorseTable['．'] = MorseTable['.'];
			MorseTable['，'] = MorseTable[','];
			MorseTable['：'] = MorseTable[':'];
			MorseTable['？'] = MorseTable['?'];
			MorseTable['’'] = MorseTable['\''];
			MorseTable['－'] = MorseTable['-'];
			MorseTable['（'] = MorseTable['('];
			MorseTable['）'] = MorseTable[')'];
			MorseTable['／'] = MorseTable['/'];
			MorseTable['＝'] = MorseTable['='];
			MorseTable['＋'] = MorseTable['+'];
			MorseTable['”'] = MorseTable['\"'];
			MorseTable['＊'] = MorseTable['*'];
			MorseTable['＠'] = MorseTable['@'];

			// 数字全角
			MorseTable['１'] = MorseTable['1'];
			MorseTable['２'] = MorseTable['2'];
			MorseTable['３'] = MorseTable['3'];
			MorseTable['４'] = MorseTable['4'];
			MorseTable['５'] = MorseTable['5'];
			MorseTable['６'] = MorseTable['6'];
			MorseTable['７'] = MorseTable['7'];
			MorseTable['８'] = MorseTable['8'];
			MorseTable['９'] = MorseTable['9'];
			MorseTable['０'] = MorseTable['0'];
		}
		#endregion

		private void morsegen_Load(object sender, System.EventArgs e)
		{
			MorseTable_Init();
			RefreshValueLabel();
		}

		private void RefreshValueLabel()
		{
			NoteNumberValueLabel_Refresh();
			VelocityValueLabel_Refresh();
			VolumeValueLabel_Refresh();
			ReverbValueLabel_Refresh();
			ChorusValueLabel_Refresh();
		}

		private void NoteNumberValueLabel_Refresh()
		{
			uint NoteNumber = (uint)NoteNumberTrackBar.Value;
			uint NoteNumberTemp = NoteNumber + (NoteNumber / 12 * 2);
			if(NoteNumber % 12 >= 5)
			{
				NoteNumberTemp++;
			}
			uint KeyNumber = (NoteNumberTemp / 2 + 2) % 7;
			bool IsSharp = ((NoteNumberTemp & 1) == 1);
			int Octave = (int)(NoteNumberTemp / 14) - 1;

			NoteNumberValueLabel.Text = "" + (char)('A' + KeyNumber) + (IsSharp ? "#" : " ") + " " + Octave;
		}

		private void VelocityValueLabel_Refresh()
		{
			VelocityValueLabel.Text = VelocityTrackBar.Value.ToString();
		}
		
		private void VolumeValueLabel_Refresh()
		{
			VolumeValueLabel.Text = VolumeTrackBar.Value.ToString();
		}

		private void ReverbValueLabel_Refresh()
		{
			ReverbValueLabel.Text = ReverbTrackBar.Value.ToString();
		}

		private void ChorusValueLabel_Refresh()
		{
			ChorusValueLabel.Text = ChorusTrackBar.Value.ToString();
		}

		private void NoteNumberTrackBar_ValueChanged(object sender, System.EventArgs e)
		{		
			NoteNumberValueLabel_Refresh();
		}

		private void VelocityTrackBar_ValueChanged(object sender, System.EventArgs e)
		{
			VelocityValueLabel_Refresh();
		}

		private void VolumeTrackBar_ValueChanged(object sender, System.EventArgs e)
		{
			VolumeValueLabel_Refresh();
		}

		private void ReverbTrackBar_ValueChanged(object sender, System.EventArgs e)
		{
			ReverbValueLabel_Refresh();
		}

		private void ChorusTrackBar_ValueChanged(object sender, System.EventArgs e)
		{
			ChorusValueLabel_Refresh();
		}

		private void SaveButton_Click(object sender, System.EventArgs e)
		{
			Random rnd = new Random();
			uint Division = 960;
			bool Compress = true;
			string MorseTextC = MsgTextBox.Text;
			string MorseText = MorseTextC.ToLower();
			uint Tick = 0;
			uint TickPerMeasure = Division * 4;
			uint MorseWpm = uint.Parse(WpmTextBox.Text);
			bool TitleIsMsg = TitleIsMsgCheckBox.Checked;
			bool GMLevel2 = GM2ResetRadioButton.Checked;
			bool GSReset = GSResetCheckBox.Checked;
			bool XGReset = XGResetCheckBox.Checked;
			int iBankMsb = int.Parse(BankMsbTextBox.Text);
			int iBankLsb = int.Parse(BankLsbTextBox.Text);
			byte BankMsb = (byte)iBankMsb;
			byte BankLsb = (byte)iBankLsb;
			int iProgram = int.Parse(PatchNumberTextBox.Text) - 1;
			byte Program = (byte)iProgram;
			byte Volume = (byte)VolumeTrackBar.Value;
			bool RndPan = RndPanCheckBox.Checked;
			byte Exp = 127;
			byte Reverb = (byte)ReverbTrackBar.Value;
			byte Chorus = (byte)ChorusTrackBar.Value;
			byte NoteNumber = (byte)NoteNumberTrackBar.Value;
			byte Velocity = (byte)VelocityTrackBar.Value;

			// エラー処理（涙）
			if(MorseWpm == 0)
			{
				MessageBox.Show("速度を" + MorseWpm + "にすることはできません");
				return;
			}
			else if(0 > iBankMsb || iBankMsb > 127 || 0 > iBankLsb || iBankLsb > 127)
			{
				MessageBox.Show("バンクナンバーには0から127までの値を入力してください");
				return;
			}
			else if(0 > iProgram || iProgram > 127)
			{
				MessageBox.Show("プログラムナンバーには1から128までの値を入力してください");
				return;
			}

			DialogResult SaveFileDialogResult = SaveFileDialog.ShowDialog();
			if(SaveFileDialogResult == DialogResult.OK)
			{
				string MorseFileName = SaveFileDialog.FileName;
				IntPtr Smf = smfw.CreateSmf((short)(Division));
				if(!Smf.Equals(null))
				{
					// シーケンス情報
					if(TitleIsMsg)
					{
						smfw.InsertMetaTextEvent(Smf, 0, 0, smfw.SMF_META_SEQUENCE_NAME, MorseTextC);
					}
					else
					{
						smfw.InsertMetaTextEvent(Smf, 0, 0, smfw.SMF_META_SEQUENCE_NAME, "signal by morsegen");
					}

					// セットアップ小節
					uint SetupTick = Division;
					uint SetupStep = Division / 16;
					smfw.InsertMetaTextEvent(Smf, Tick, 0, smfw.SMF_META_MARKER, "Setup");
					smfw.MEvInsertTimeSignature(Smf, Tick, 0, 1, 4, 24, 8);
					smfw.MEvInsertSetTempo(Smf, Tick, 0, (uint)(60 * 1000000 / (MorseWpm * 6.25)));
					if(GMLevel2)
					{
						smfw.XEvInsertGM2SystemOn(Smf, Tick, 0, 0);     Tick += SetupStep;
					}
					else
					{
						smfw.XEvInsertGMSystemOn(Smf, Tick, 0, 0);      Tick += SetupStep;
					}
					if(GSReset)
					{
						smfw.XEvInsertGSReset(Smf, Tick, 0, 0);         Tick += SetupStep;
					}
					if(XGReset)
					{
						smfw.XEvInsertXGSystemOn(Smf, Tick, 0, 0);      Tick += SetupStep;
					}
					smfw.EvInsertControl(Smf, Tick, 0, 0, 0, BankMsb);  Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 32, BankLsb); Tick += SetupStep;
					smfw.EvInsertPatch(Smf, Tick, 0, 0, Program);       Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 7, Volume);   Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 10, !RndPan ? 64 : (uint)rnd.Next(128)); Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 11, Exp);     Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 91, Reverb);  Tick += SetupStep;
					smfw.EvInsertControl(Smf, Tick, 0, 0, 93, Chorus);  Tick += SetupStep;
					Tick = Division;
					smfw.InsertMetaTextEvent(Smf, Tick, 0, smfw.SMF_META_MARKER, "Start");
					smfw.MEvInsertTimeSignature(Smf, Tick, 0, 4, 4, 24, 8);

					uint MorsePulseLen = Division / 8;
					uint MorseLongPulseLen = MorsePulseLen * 3;
					bool Result = true;
					bool FirstNote = true;
					// SOS例外処理
					if(MorseText == "sos")
					{
						// 緊急事態だ！コードの形式など気にしている場合ではない！
						string MorseSig = "...___...";
						for(int SigIndex = 0; SigIndex < MorseSig.Length; SigIndex++)
						{
							char SigChar = MorseSig[SigIndex];
							switch(SigChar)
							{
								case '.':
									smfw.EvInsertNote(Smf, Tick, 0, 0, NoteNumber, Velocity, MorsePulseLen);
									Tick += MorsePulseLen;
									break;
								case '_':
									smfw.EvInsertNote(Smf, Tick, 0, 0, NoteNumber, Velocity, MorseLongPulseLen);
									Tick += MorseLongPulseLen;
									break;
								case ' ':
									Tick += (MorseLongPulseLen - MorsePulseLen * 2);
									break;
							}
							Tick += MorsePulseLen;
						}
						Tick += (MorseLongPulseLen - MorsePulseLen);
					}
					else
					{
						// 信号の記録
						for(int Index = 0; Index < MorseText.Length; Index++)
						{
							char MorseChar = MorseText[Index];
							if(char.IsWhiteSpace(MorseChar))
							{
								Tick += MorsePulseLen + MorseLongPulseLen;
							}
							else
							{
								string MorseSig = (string)MorseTable[MorseChar];
								if(MorseSig != null)
								{
									if(!FirstNote)
									{
										if(RndPan)
										{
											smfw.EvInsertControl(Smf, Tick - 1, 0, 0, 10, (uint)rnd.Next(128));
										}
									}
									else
									{
										FirstNote = false;
									}

									for(int SigIndex = 0; SigIndex < MorseSig.Length; SigIndex++)
									{
										char SigChar = MorseSig[SigIndex];
										switch(SigChar)
										{
											case '.':
												smfw.EvInsertNote(Smf, Tick, 0, 0, NoteNumber, Velocity, MorsePulseLen);
												Tick += MorsePulseLen;
												break;
											case '_':
												smfw.EvInsertNote(Smf, Tick, 0, 0, NoteNumber, Velocity, MorseLongPulseLen);
												Tick += MorseLongPulseLen;
												break;
											case ' ':
												Tick += (MorseLongPulseLen - MorsePulseLen * 2);
												break;
										}
										Tick += MorsePulseLen;
									}
									Tick += (MorseLongPulseLen - MorsePulseLen);
								}
								else
								{
									MessageBox.Show(MorseChar.ToString() + " を変換できません");
									Result = false;
									break;
								}
							}
						}
					}

					// 信号の書き出し
					if(Result)
					{
						uint ModTick = (Tick - SetupTick) % TickPerMeasure;
						uint PaddingTick = (ModTick == 0) ? 0 : (TickPerMeasure - ModTick);
						Tick += PaddingTick + TickPerMeasure;
						smfw.SetEndOfTrack(Smf, 0, Tick);
						smfw.WriteFile(Smf, MorseFileName, 0, Compress, 2, 0x80);
						smfw.DeleteSmf(Smf);
					}
				}
			}
		}
	}

	// オブジェクト指向とは思えないクラス定義（笑）
	public class smfw
	{
		public const uint smfwVersion = 0x0100;
		public const byte SMF_CTRL_BANK_SELECT_MSB  = 0;
		public const byte SMF_CTRL_MODULATION_DEPTH = 1;
		public const byte SMF_CTRL_PORTAMENTO_TIME  = 5;
		public const byte SMF_CTRL_DATA_ENTRY_MSB   = 6;
		public const byte SMF_CTRL_VOLUME           = 7;
		public const byte SMF_CTRL_PANPOT           = 10;
		public const byte SMF_CTRL_EXPRESSION       = 11;
		public const byte SMF_CTRL_BANK_SELECT_LSB  = 32;
		public const byte SMF_CTRL_DATA_ENTRY_LSB   = 38;
		public const byte SMF_CTRL_HOLD             = 64;
		public const byte SMF_CTRL_PORTAMENTO       = 65;
		public const byte SMF_CTRL_SOSTENUTO        = 66;
		public const byte SMF_CTRL_SOFT             = 67;
		public const byte SMF_CTRL_FILTER_RESONANCE = 71;
		public const byte SMF_CTRL_RELEASE_TIME     = 72;
		public const byte SMF_CTRL_ATTACK_TIME      = 73;
		public const byte SMF_CTRL_BRIGHTNESS       = 74;
		public const byte SMF_CTRL_DECAY_TIME       = 75;
		public const byte SMF_CTRL_VIBRATO_RATE     = 76;
		public const byte SMF_CTRL_VIBRATO_DEPTH    = 77;
		public const byte SMF_CTRL_VIBRATO_DELAY    = 78;
		public const byte SMF_CTRL_REVERB           = 91;
		public const byte SMF_CTRL_CHORUS           = 93;
		public const byte SMF_CTRL_NRPN_LSB         = 98;
		public const byte SMF_CTRL_NRPN_MSB         = 99;
		public const byte SMF_CTRL_RPN_LSB          = 100;
		public const byte SMF_CTRL_RPN_MSB          = 101;
		public const byte SMF_CTRL_ALL_SOUND_OFF    = 120;
		public const byte SMF_CTRL_RESET_ALL_CTRL   = 121;
		public const byte SMF_CTRL_ALL_NOTE_OFF     = 123;
		public const byte SMF_CTRL_OMNI_MODE_OFF    = 124;
		public const byte SMF_CTRL_OMNI_MODE_ON     = 125;
		public const byte SMF_CTRL_MONO_MODE_ON     = 126;
		public const byte SMF_CTRL_POLY_MODE_ON     = 127;

		public const byte SMF_META_SEQUENCE_NUMBER  = 0x00;
		public const byte SMF_META_TEXT             = 0x01;
		public const byte SMF_META_COPYRIGHT        = 0x02;
		public const byte SMF_META_SEQUENCE_NAME    = 0x03;
		public const byte SMF_META_TRACK_NAME       = SMF_META_SEQUENCE_NAME;
		public const byte SMF_META_INSTRUMENT_NAME  = 0x04;
		public const byte SMF_META_LYRIC            = 0x05;
		public const byte SMF_META_MARKER           = 0x06;
		public const byte SMF_META_CUE_POINT        = 0x07;
		public const byte SMF_META_PROGRAM_NAME     = 0x08;
		public const byte SMF_META_DEVICE_NAME      = 0x09;
		public const byte SMF_META_CHANNEL_PREFIX   = 0x20;
		public const byte SMF_META_END_OF_TRACK     = 0x2F;
		public const byte SMF_META_SET_TEMPO        = 0x51;
		public const byte SMF_META_SMPTE_OFFSET     = 0x54;
		public const byte SMF_META_TIME_SIGNATURE   = 0x58;
		public const byte SMF_META_KEY_SIGNATURE    = 0x59;
		public const byte SMF_META_SEQUENCER_SPECIFIC = 0x7F;

		public const uint SMPTE_TIMECODE_24         = 0;
		public const uint SMPTE_TIMECODE_25         = 1;
		public const uint SMPTE_TIMECODE_30DROP     = 2;
		public const uint SMPTE_TIMECODE_30NONDROP  = 3;

		[DllImport("smfw32.dll" , EntryPoint="smfwGetVersion")]
		public static extern uint GetVersion();
		[DllImport("smfw32.dll" , EntryPoint="smfwCreateSmf")]
		public static extern IntPtr CreateSmf(short Division);
		[DllImport("smfw32.dll" , EntryPoint="smfwDeleteSmf")]
		public static extern bool DeleteSmf(IntPtr hSmfw);
		[DllImport("smfw32.dll" , EntryPoint="smfwCopySmf")]
		public static extern IntPtr CopySmf(IntPtr hSmfw);
		[DllImport("smfw32.dll" , EntryPoint="smfwReadMem")]
		public static extern IntPtr ReadMem(ref byte[] Data, uint Size);
		[DllImport("smfw32.dll" , EntryPoint="smfwReadFileA")]
		public static extern IntPtr ReadFile(string FileName);
		[DllImport("smfw32.dll" , EntryPoint="smfwIsClipboardFormatAvailable")]
		public static extern bool IsClipboardFormatAvailable();
		[DllImport("smfw32.dll" , EntryPoint="smfwGetClipboardData")]
		public static extern IntPtr GetClipboardData();
		[DllImport("smfw32.dll" , EntryPoint="smfwGetSmfSize")]
		public static extern uint GetSmfSize(IntPtr hSmfw, ushort Format, bool UseRunningStatus);
		[DllImport("smfw32.dll" , EntryPoint="smfwWriteMem")]
		public static extern bool WriteMem(IntPtr hSmfw, ref byte[] Buffer, uint Size, ushort Format, bool UseRunningStatus);
		[DllImport("smfw32.dll" , EntryPoint="smfwWriteFileA")]
		public static extern bool WriteFile(IntPtr hSmfw, string FileName, ushort Format, bool UseRunningStatus, uint CreationDisposition, uint FlagsAndAttributes);
		[DllImport("smfw32.dll" , EntryPoint="smfwSetClipboardData")]
		public static extern bool SetClipboardData(IntPtr hSmfw, ushort Format, bool UseRunningStatus);

		[DllImport("smfw32.dll" , EntryPoint="smfwSetEndOfTrack")]
		public static extern bool SetEndOfTrack(IntPtr hSmfw, uint Track, uint Tick);
		[DllImport("smfw32.dll" , EntryPoint="smfwInsertEvent")]
		public static extern bool InsertEvent(IntPtr hSmfw, uint Tick, uint Channel, uint Track, ref byte[] Data, uint Size);
		[DllImport("smfw32.dll" , EntryPoint="smfwInsertSysex")]
		public static extern bool InsertSysex(IntPtr hSmfw, uint Tick, byte Port, uint Track, ref byte[] Data, uint Size);
		[DllImport("smfw32.dll" , EntryPoint="smfwInsertMetaEvent")]
		public static extern bool InsertMetaEvent(IntPtr hSmfw, uint Tick, uint Track, byte MetaType, ref byte[] Data, uint Size);
		[DllImport("smfw32.dll" , EntryPoint="smfwInsertMetaTextEventA")]
		public static extern bool InsertMetaTextEvent(IntPtr hSmfw, uint Tick, uint Track, byte MetaType, string Text);

		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertNoteOff")]
		public static extern bool EvInsertNoteOff(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint Key, uint Velocity);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertNoteOn")]
		public static extern bool EvInsertNoteOn(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint Key, uint Velocity);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertNote")]
		public static extern bool EvInsertNote(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint Key, uint Velocity, uint Duration);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertKeyAft")]
		public static extern bool EvInsertKeyAft(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint Key, uint Amount);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertControl")]
		public static extern bool EvInsertControl(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint ControlNumber, uint Value);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertPatch")]
		public static extern bool EvInsertPatch(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint PatchNumber);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertChanAft")]
		public static extern bool EvInsertChanAft(IntPtr hSmfw, uint Tick, uint Channel, uint Track, uint Key, uint Amount);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertWheel")]
		public static extern bool EvInsertWheel(IntPtr hSmfw, uint Tick, uint Channel, uint Track, int Value);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertSysexA")]
		public static extern bool EvInsertSysex(IntPtr hSmfw, uint Tick, byte Port, uint Track, string Text);
		[DllImport("smfw32.dll" , EntryPoint="smfwEvInsertMetaEventA")]
		public static extern bool EvInsertMetaEvent(IntPtr hSmfw, uint Tick, uint Track, string Text);

		[DllImport("smfw32.dll" , EntryPoint="smfwXEvInsertGMSystemOn")]
		public static extern bool XEvInsertGMSystemOn(IntPtr hSmfw, uint Tick, byte Port, uint Track);
		[DllImport("smfw32.dll" , EntryPoint="smfwXEvInsertGM2SystemOn")]
		public static extern bool XEvInsertGM2SystemOn(IntPtr hSmfw, uint Tick, byte Port, uint Track);
		[DllImport("smfw32.dll" , EntryPoint="smfwXEvInsertGSReset")]
		public static extern bool XEvInsertGSReset(IntPtr hSmfw, uint Tick, byte Port, uint Track);
		[DllImport("smfw32.dll" , EntryPoint="smfwXEvInsertXGSystemOn")]
		public static extern bool XEvInsertXGSystemOn(IntPtr hSmfw, uint Tick, byte Port, uint Track);
		[DllImport("smfw32.dll" , EntryPoint="smfwXEvInsertMasterVolume")]
		public static extern bool XEvInsertMasterVolume(IntPtr hSmfw, uint Tick, byte Port, uint Track, uint Value);

		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertChannelPrefix")]
		public static extern bool MEvInsertChannelPrefix(IntPtr hSmfw, uint Tick, uint Track, uint Channel);
		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertSetTempo")]
		public static extern bool MEvInsertSetTempo(IntPtr hSmfw, uint Tick, uint Track, uint LengthOfQuarterNote);
		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertSetBpmTempo")]
		public static extern bool MEvInsertSetBpmTempo(IntPtr hSmfw, uint Tick, uint Track, uint Bpm);
		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertSMPTEOffset")]
		public static extern bool MEvInsertSMPTEOffset(IntPtr hSmfw, uint Tick, uint Track, uint Hour, uint Minute, uint Second, uint Frame, uint SubFrame, uint TimeCodeType);
		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertTimeSignature")]
		public static extern bool MEvInsertTimeSignature(IntPtr hSmfw, uint Tick, uint Track, uint Numerator, uint Denominator, uint ClocksPerQuarter, uint NotesPerQuarter);
		[DllImport("smfw32.dll" , EntryPoint="smfwMEvInsertKeySignature")]
		public static extern bool MEvInsertKeySignature(IntPtr hSmfw, uint Tick, uint Track, int SharpFlatCount, bool IsMinor);
	}
}
