namespace TankMapEditor2
{
    partial class frmMainEditor
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
            this.btnNew = new System.Windows.Forms.Button();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txtAttributes = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnZeroAttr = new System.Windows.Forms.Button();
            this.btnAttrUpdate = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.rbActiveEntities = new System.Windows.Forms.RadioButton();
            this.rbActiveGround = new System.Windows.Forms.RadioButton();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.cbIncrementAttr = new System.Windows.Forms.CheckBox();
            this.rbSet = new System.Windows.Forms.RadioButton();
            this.rbQuery = new System.Windows.Forms.RadioButton();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.cbVisibleEntities = new System.Windows.Forms.CheckBox();
            this.panelMap = new TankMapEditor2.DoubleBufferPanel();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBoxCellTypes
            // 
            this.comboBoxCellTypes.FormattingEnabled = true;
            this.comboBoxCellTypes.Items.AddRange(new object[] {
            "Wall - Ground",
            "Floor1- Ground",
            "Floor2 - Ground",
            "Base1 - Entity",
            "Base2 - Entity",
            "Turret1 - Entity",
            "Turret2 - Entity",
            "Waypoint1_Team1 - Entity",
            "Waypoint2_Team1 - Entity",
            "Waypoint3_Team1 - Entity",
            "Waypoint4_Team1 - Entity",
            "Waypoint5_Team1 - Entity",
            "Waypoint6_Team1 - Entity",
            "Waypoint1_Team2 - Entity",
            "Waypoint2_Team2 - Entity",
            "Waypoint3_Team2 - Entity",
            "Waypoint4_Team2 - Entity",
            "Waypoint5_Team2 - Entity",
            "Waypoint6_Team2 - Entity",
            "Totem1_Team1 - Entity",
            "Totem1_Team2 - Entity"});
            this.comboBoxCellTypes.Location = new System.Drawing.Point(6, 17);
            this.comboBoxCellTypes.Name = "comboBoxCellTypes";
            this.comboBoxCellTypes.Size = new System.Drawing.Size(162, 21);
            this.comboBoxCellTypes.TabIndex = 0;
            this.comboBoxCellTypes.SelectedIndexChanged += new System.EventHandler(this.comboBoxCellTypes_SelectedIndexChanged);
            // 
            // lblMouseCoordinates
            // 
            this.lblMouseCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblMouseCoordinates.AutoSize = true;
            this.lblMouseCoordinates.Location = new System.Drawing.Point(-2, 514);
            this.lblMouseCoordinates.Name = "lblMouseCoordinates";
            this.lblMouseCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblMouseCoordinates.TabIndex = 0;
            this.lblMouseCoordinates.Text = "label1";
            // 
            // lblGridCoordinates
            // 
            this.lblGridCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblGridCoordinates.AutoSize = true;
            this.lblGridCoordinates.Location = new System.Drawing.Point(-2, 536);
            this.lblGridCoordinates.Name = "lblGridCoordinates";
            this.lblGridCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblGridCoordinates.TabIndex = 1;
            this.lblGridCoordinates.Text = "label2";
            // 
            // lblMapCoordinates
            // 
            this.lblMapCoordinates.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblMapCoordinates.AutoSize = true;
            this.lblMapCoordinates.Location = new System.Drawing.Point(-2, 560);
            this.lblMapCoordinates.Name = "lblMapCoordinates";
            this.lblMapCoordinates.Size = new System.Drawing.Size(35, 13);
            this.lblMapCoordinates.TabIndex = 2;
            this.lblMapCoordinates.Text = "label3";
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSave.Location = new System.Drawing.Point(6, 104);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(162, 23);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnFill
            // 
            this.btnFill.Location = new System.Drawing.Point(6, 44);
            this.btnFill.Name = "btnFill";
            this.btnFill.Size = new System.Drawing.Size(162, 23);
            this.btnFill.TabIndex = 3;
            this.btnFill.Text = "Fill";
            this.btnFill.UseVisualStyleBackColor = true;
            this.btnFill.Click += new System.EventHandler(this.btnFill_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnLoad.Location = new System.Drawing.Point(6, 75);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(162, 23);
            this.btnLoad.TabIndex = 4;
            this.btnLoad.Text = "Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnNew
            // 
            this.btnNew.Location = new System.Drawing.Point(6, 46);
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(162, 23);
            this.btnNew.TabIndex = 5;
            this.btnNew.Text = "New";
            this.btnNew.UseVisualStyleBackColor = true;
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
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
            this.txtHeight.Location = new System.Drawing.Point(116, 20);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(52, 20);
            this.txtHeight.TabIndex = 7;
            this.txtHeight.Text = "61";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(84, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "x";
            // 
            // txtAttributes
            // 
            this.txtAttributes.Location = new System.Drawing.Point(6, 19);
            this.txtAttributes.Name = "txtAttributes";
            this.txtAttributes.Size = new System.Drawing.Size(148, 20);
            this.txtAttributes.TabIndex = 8;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnZeroAttr);
            this.groupBox1.Controls.Add(this.btnAttrUpdate);
            this.groupBox1.Controls.Add(this.txtAttributes);
            this.groupBox1.Location = new System.Drawing.Point(1, 285);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(174, 79);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Attributes";
            // 
            // btnZeroAttr
            // 
            this.btnZeroAttr.Location = new System.Drawing.Point(158, 19);
            this.btnZeroAttr.Name = "btnZeroAttr";
            this.btnZeroAttr.Size = new System.Drawing.Size(10, 20);
            this.btnZeroAttr.TabIndex = 0;
            this.btnZeroAttr.UseVisualStyleBackColor = true;
            this.btnZeroAttr.Click += new System.EventHandler(this.btnZeroAttr_Click);
            // 
            // btnAttrUpdate
            // 
            this.btnAttrUpdate.Location = new System.Drawing.Point(5, 45);
            this.btnAttrUpdate.Name = "btnAttrUpdate";
            this.btnAttrUpdate.Size = new System.Drawing.Size(163, 24);
            this.btnAttrUpdate.TabIndex = 9;
            this.btnAttrUpdate.Text = "Update";
            this.btnAttrUpdate.UseVisualStyleBackColor = true;
            this.btnAttrUpdate.Click += new System.EventHandler(this.btnAttrUpdate_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.btnNew);
            this.groupBox2.Controls.Add(this.txtWidth);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.txtHeight);
            this.groupBox2.Controls.Add(this.btnLoad);
            this.groupBox2.Controls.Add(this.btnSave);
            this.groupBox2.Location = new System.Drawing.Point(1, 370);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(174, 132);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Map";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.rbActiveEntities);
            this.groupBox3.Controls.Add(this.rbActiveGround);
            this.groupBox3.Location = new System.Drawing.Point(1, 98);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(81, 69);
            this.groupBox3.TabIndex = 0;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Active";
            // 
            // rbActiveEntities
            // 
            this.rbActiveEntities.AutoSize = true;
            this.rbActiveEntities.Location = new System.Drawing.Point(5, 42);
            this.rbActiveEntities.Name = "rbActiveEntities";
            this.rbActiveEntities.Size = new System.Drawing.Size(59, 17);
            this.rbActiveEntities.TabIndex = 1;
            this.rbActiveEntities.Text = "Entities";
            this.rbActiveEntities.UseVisualStyleBackColor = true;
            // 
            // rbActiveGround
            // 
            this.rbActiveGround.AutoSize = true;
            this.rbActiveGround.Checked = true;
            this.rbActiveGround.Location = new System.Drawing.Point(5, 19);
            this.rbActiveGround.Name = "rbActiveGround";
            this.rbActiveGround.Size = new System.Drawing.Size(60, 17);
            this.rbActiveGround.TabIndex = 0;
            this.rbActiveGround.TabStop = true;
            this.rbActiveGround.Text = "Ground";
            this.rbActiveGround.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.cbIncrementAttr);
            this.groupBox4.Controls.Add(this.rbSet);
            this.groupBox4.Controls.Add(this.rbQuery);
            this.groupBox4.Location = new System.Drawing.Point(1, 188);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(174, 91);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Mode";
            // 
            // cbIncrementAttr
            // 
            this.cbIncrementAttr.AutoSize = true;
            this.cbIncrementAttr.Location = new System.Drawing.Point(5, 64);
            this.cbIncrementAttr.Name = "cbIncrementAttr";
            this.cbIncrementAttr.Size = new System.Drawing.Size(92, 17);
            this.cbIncrementAttr.TabIndex = 0;
            this.cbIncrementAttr.Text = "Increment Attr";
            this.cbIncrementAttr.UseVisualStyleBackColor = true;
            // 
            // rbSet
            // 
            this.rbSet.AutoSize = true;
            this.rbSet.Checked = true;
            this.rbSet.Location = new System.Drawing.Point(5, 41);
            this.rbSet.Name = "rbSet";
            this.rbSet.Size = new System.Drawing.Size(41, 17);
            this.rbSet.TabIndex = 11;
            this.rbSet.TabStop = true;
            this.rbSet.Text = "Set";
            this.rbSet.UseVisualStyleBackColor = true;
            this.rbSet.CheckedChanged += new System.EventHandler(this.rbSet_CheckedChanged);
            // 
            // rbQuery
            // 
            this.rbQuery.AutoSize = true;
            this.rbQuery.Location = new System.Drawing.Point(5, 18);
            this.rbQuery.Name = "rbQuery";
            this.rbQuery.Size = new System.Drawing.Size(53, 17);
            this.rbQuery.TabIndex = 0;
            this.rbQuery.Text = "Query";
            this.rbQuery.UseVisualStyleBackColor = true;
            this.rbQuery.CheckedChanged += new System.EventHandler(this.rbSet_CheckedChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.comboBoxCellTypes);
            this.groupBox5.Controls.Add(this.btnFill);
            this.groupBox5.Location = new System.Drawing.Point(1, 12);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(174, 73);
            this.groupBox5.TabIndex = 0;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Tile Brush";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.cbVisibleEntities);
            this.groupBox6.Location = new System.Drawing.Point(88, 98);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(87, 69);
            this.groupBox6.TabIndex = 2;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Visible";
            // 
            // cbVisibleEntities
            // 
            this.cbVisibleEntities.AutoSize = true;
            this.cbVisibleEntities.Checked = true;
            this.cbVisibleEntities.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbVisibleEntities.Location = new System.Drawing.Point(7, 19);
            this.cbVisibleEntities.Name = "cbVisibleEntities";
            this.cbVisibleEntities.Size = new System.Drawing.Size(60, 17);
            this.cbVisibleEntities.TabIndex = 0;
            this.cbVisibleEntities.Text = "Entities";
            this.cbVisibleEntities.UseVisualStyleBackColor = true;
            this.cbVisibleEntities.CheckedChanged += new System.EventHandler(this.cbVisibleEntities_CheckedChanged);
            // 
            // panelMap
            // 
            this.panelMap.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelMap.Location = new System.Drawing.Point(181, 12);
            this.panelMap.Name = "panelMap";
            this.panelMap.Size = new System.Drawing.Size(971, 562);
            this.panelMap.TabIndex = 11;
            this.panelMap.Paint += new System.Windows.Forms.PaintEventHandler(this.panelMap_Paint);
            this.panelMap.MouseClick += new System.Windows.Forms.MouseEventHandler(this.panelMap_MouseMove);
            this.panelMap.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panelMap_MouseMove);
            // 
            // frmMainEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1152, 574);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.panelMap);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.lblMapCoordinates);
            this.Controls.Add(this.lblGridCoordinates);
            this.Controls.Add(this.lblMouseCoordinates);
            this.Name = "frmMainEditor";
            this.Text = "Mega Blaster Map Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
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
        private System.Windows.Forms.Button btnNew;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtAttributes;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnAttrUpdate;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton rbActiveEntities;
        private System.Windows.Forms.RadioButton rbActiveGround;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.RadioButton rbSet;
        private System.Windows.Forms.RadioButton rbQuery;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.CheckBox cbIncrementAttr;
        private System.Windows.Forms.Button btnZeroAttr;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox cbVisibleEntities;
    }
}

