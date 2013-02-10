namespace TankMapMaker
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.comboBoxCellTypes = new System.Windows.Forms.ComboBox();
            this.lblMouseCoordinates = new System.Windows.Forms.Label();
            this.lblGridCoordinates = new System.Windows.Forms.Label();
            this.lblMapCoordinates = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnFill = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txtAttributes = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnAttrUpdate = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.panelMap = new TankMapMaker.DoubleBufferPanel();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBoxCellTypes
            // 
            this.comboBoxCellTypes.FormattingEnabled = true;
            this.comboBoxCellTypes.Items.AddRange(new object[] {
            "Wall",
            "Floor1",
            "Floor2",
            "Base1",
            "Base2",
            "Turret1",
            "Turret2",
            "Waypoint1_Team1",
            "Waypoint2_Team1",
            "Waypoint3_Team1",
            "Waypoint1_Team2",
            "Waypoint2_Team2",
            "Waypoint3_Team2"});
            this.comboBoxCellTypes.Location = new System.Drawing.Point(1, 12);
            this.comboBoxCellTypes.Name = "comboBoxCellTypes";
            this.comboBoxCellTypes.Size = new System.Drawing.Size(153, 21);
            this.comboBoxCellTypes.TabIndex = 0;
            this.comboBoxCellTypes.SelectedIndexChanged += new System.EventHandler(this.comboBoxCellTypes_SelectedIndexChanged);
            // 
            // lblMouseCoordinates
            // 
            this.lblMouseCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblMouseCoordinates.AutoSize = true;
            this.lblMouseCoordinates.Location = new System.Drawing.Point(-2, 444);
            this.lblMouseCoordinates.Name = "lblMouseCoordinates";
            this.lblMouseCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblMouseCoordinates.TabIndex = 0;
            this.lblMouseCoordinates.Text = "label1";
            // 
            // lblGridCoordinates
            // 
            this.lblGridCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblGridCoordinates.AutoSize = true;
            this.lblGridCoordinates.Location = new System.Drawing.Point(-2, 466);
            this.lblGridCoordinates.Name = "lblGridCoordinates";
            this.lblGridCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblGridCoordinates.TabIndex = 1;
            this.lblGridCoordinates.Text = "label2";
            // 
            // lblMapCoordinates
            // 
            this.lblMapCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblMapCoordinates.AutoSize = true;
            this.lblMapCoordinates.Location = new System.Drawing.Point(-2, 490);
            this.lblMapCoordinates.Name = "lblMapCoordinates";
            this.lblMapCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblMapCoordinates.TabIndex = 2;
            this.lblMapCoordinates.Text = "label3";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(1, 417);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(153, 23);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnFill
            // 
            this.btnFill.Location = new System.Drawing.Point(1, 39);
            this.btnFill.Name = "btnFill";
            this.btnFill.Size = new System.Drawing.Size(153, 23);
            this.btnFill.TabIndex = 3;
            this.btnFill.Text = "Fill";
            this.btnFill.UseVisualStyleBackColor = true;
            this.btnFill.Click += new System.EventHandler(this.btnFill_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(1, 388);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(153, 23);
            this.btnLoad.TabIndex = 4;
            this.btnLoad.Text = "Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(6, 46);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(141, 23);
            this.btnSet.TabIndex = 5;
            this.btnSet.Text = "Update";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // txtWidth
            // 
            this.txtWidth.Location = new System.Drawing.Point(13, 20);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new System.Drawing.Size(52, 20);
            this.txtWidth.TabIndex = 6;
            this.txtWidth.Text = "121";
            // 
            // txtHeight
            // 
            this.txtHeight.Location = new System.Drawing.Point(85, 19);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(52, 20);
            this.txtHeight.TabIndex = 7;
            this.txtHeight.Text = "61";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(69, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "x";
            // 
            // txtAttributes
            // 
            this.txtAttributes.Location = new System.Drawing.Point(6, 19);
            this.txtAttributes.Name = "txtAttributes";
            this.txtAttributes.Size = new System.Drawing.Size(135, 20);
            this.txtAttributes.TabIndex = 8;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnAttrUpdate);
            this.groupBox1.Controls.Add(this.txtAttributes);
            this.groupBox1.Location = new System.Drawing.Point(1, 68);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(153, 85);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Attributes";
            // 
            // btnAttrUpdate
            // 
            this.btnAttrUpdate.Location = new System.Drawing.Point(5, 45);
            this.btnAttrUpdate.Name = "btnAttrUpdate";
            this.btnAttrUpdate.Size = new System.Drawing.Size(142, 24);
            this.btnAttrUpdate.TabIndex = 9;
            this.btnAttrUpdate.Text = "Update";
            this.btnAttrUpdate.UseVisualStyleBackColor = true;
            this.btnAttrUpdate.Click += new System.EventHandler(this.btnAttrUpdate_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnSet);
            this.groupBox2.Controls.Add(this.txtWidth);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.txtHeight);
            this.groupBox2.Location = new System.Drawing.Point(1, 300);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(153, 82);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "New";
            // 
            // panelMap
            // 
            this.panelMap.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelMap.Location = new System.Drawing.Point(160, 12);
            this.panelMap.Name = "panelMap";
            this.panelMap.Size = new System.Drawing.Size(973, 492);
            this.panelMap.TabIndex = 0;
            this.panelMap.Paint += new System.Windows.Forms.PaintEventHandler(this.panelMap_Paint);
            this.panelMap.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panelMap_MouseDown);
            this.panelMap.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panelMap_MouseMove);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1133, 504);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnFill);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.lblMapCoordinates);
            this.Controls.Add(this.lblGridCoordinates);
            this.Controls.Add(this.lblMouseCoordinates);
            this.Controls.Add(this.comboBoxCellTypes);
            this.Controls.Add(this.panelMap);
            this.Name = "Form1";
            this.Text = "Mega Blaster Map Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private DoubleBufferPanel panelMap;
        private System.Windows.Forms.ComboBox comboBoxCellTypes;
        private System.Windows.Forms.Label lblMouseCoordinates;
        private System.Windows.Forms.Label lblGridCoordinates;
        private System.Windows.Forms.Label lblMapCoordinates;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnFill;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtAttributes;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnAttrUpdate;
        private System.Windows.Forms.GroupBox groupBox2;
    }
}

