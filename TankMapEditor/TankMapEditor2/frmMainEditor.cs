using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TankMapEditor2
{
    public partial class frmMainEditor : Form
    {
        int mapCoordX;//Last Clicked Spot
        int mapCoordY;//        "

        Map map = new Map();
        MapBrush current = null;
        Bitmap mapBmp = null;

        //Ground Tiles
        MapBrush b0 = new MapBrush("wall.png", 0, "0");//wall
        MapBrush b1 = new MapBrush("floor1.png", 1, "1");//floor 1
        MapBrush b2 = new MapBrush("floor2.png", 2, "2");//floor 2



        //Entity Tiles
        MapBrush b3 = new MapBrush("base1.png", 3, "3 1");// team 1 garage
        MapBrush b4 = new MapBrush("base2.png", 4, "3 2");//team 2 garage
        MapBrush b5 = new MapBrush("turret1.png", 5, "4 1");//team 1 minion generator
        MapBrush b6 = new MapBrush("turret2.png", 6, "4 2");//team 2 minion generator

        MapBrush b7 = new MapBrush("w1_1.png", 7, "5 1 1 0");//Item 4, team 1, WayType 1, Starting Id
        MapBrush b8 = new MapBrush("w2_1.png", 8, "5 1 2 0");//team 1 minion 2 waypoint
        MapBrush b9 = new MapBrush("w3_1.png", 9, "5 1 3 0");//team 1 minion 3 waypoint
        MapBrush b10 = new MapBrush("w4_1.png", 10, "5 1 4 0");//team 1 minion 4 waypoint
        MapBrush b11 = new MapBrush("w5_1.png", 11, "5 1 5 0");//team 1 minion 5 waypoint
        MapBrush b12 = new MapBrush("w6_1.png", 12, "5 1 6 0");//team 1 minion 6 waypoint

        MapBrush b13 = new MapBrush("w1_2.png", 13, "5 2 1 0");//team 2 minion 1 waypoint
        MapBrush b14 = new MapBrush("w2_2.png", 14, "5 2 2 0");//team 2 minion 2 waypoint
        MapBrush b15 = new MapBrush("w3_2.png", 15, "5 2 3 0");//team 2 minion 3 waypoint
        MapBrush b16 = new MapBrush("w4_2.png", 16, "5 2 4 0");//team 2 minion 4 waypoint
        MapBrush b17 = new MapBrush("w5_2.png", 17, "5 2 5 0");//team 2 minion 5 waypoint
        MapBrush b18 = new MapBrush("w6_2.png", 18, "5 2 6 0");//team 2 minion 6 waypoint

        MapBrush b19 = new MapBrush("totum1_1.png", 19, "6 1");//Item 5, Team 1 - Totum
        MapBrush b20 = new MapBrush("totum1_2.png", 20, "6 2");//Item 5, Team 2 - Totum


        public frmMainEditor()
        {
            InitializeComponent();
        }

        private void panelMap_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (map == null)
                    return;

                decimal bitmapDimX = 8;
                decimal bitmapDimY = 8;

                //Convert Mouse to Grid
                int gridCoordX = (int)Math.Floor((decimal)(e.X) / bitmapDimX) * (int)bitmapDimX;
                int gridCoordY = (int)Math.Floor((decimal)(e.Y) / bitmapDimY) * (int)bitmapDimY;
                //Skip if been there done that..
                //if (mapCoordX == gridCoordX / (int)bitmapDimX &&
                //    mapCoordY == gridCoordY / (int)bitmapDimY)
                //    return;
                //Convert Grid to Map
                mapCoordX = gridCoordX / (int)bitmapDimX;
                mapCoordY = gridCoordY / (int)bitmapDimY;

                //Print out Coordinate Info
                lblMouseCoordinates.Text = String.Format("Mouse: {0},{1}", e.X, e.Y);
                lblGridCoordinates.Text =  String.Format("Grid : {0},{1}", gridCoordX, gridCoordY);
                lblMapCoordinates.Text =   String.Format("Map  : {0},{1}", mapCoordX, mapCoordY);

                //Check for out of bounds
                if (mapCoordX >= 0 && mapCoordX < map.width)
                {
                    if (mapCoordY >= 0 && mapCoordY < map.height)
                    {
                        if (rbSet.Checked)
                        {

                            if (rbActiveGround.Checked)
                            {
                                map.groundData[mapCoordX, mapCoordY].type = current.type;
                                map.groundData[mapCoordX, mapCoordY].empty = false;
                                map.entityData[mapCoordX, mapCoordY].empty = true;
                                map.groundData[mapCoordX, mapCoordY].attributes.Clear();

                                string[] attributes = txtAttributes.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                                foreach (string a in attributes)
                                {
                                    map.groundData[mapCoordX, mapCoordY].attributes.Add(int.Parse(a));
                                }
                            }
                            else if (rbActiveEntities.Checked)
                            {
                                map.entityData[mapCoordX, mapCoordY].type = current.type;
                                map.entityData[mapCoordX, mapCoordY].empty = false;
                                map.entityData[mapCoordX, mapCoordY].attributes.Clear();

                                string[] attributes = txtAttributes.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                                foreach (string a in attributes)
                                {
                                    map.entityData[mapCoordX, mapCoordY].attributes.Add(int.Parse(a));
                                }
                            }

                            if (cbIncrementAttr.Checked)
                            {
                                string[] attributes = txtAttributes.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                                txtAttributes.Text = string.Empty;
                                int lastNum = int.Parse(attributes[attributes.Length-1]) + 1;
                                for(int s = 0;s < attributes.Length-1;s++)
                                {
                                    txtAttributes.Text += attributes[s] + " ";
                                }
                                txtAttributes.Text += lastNum.ToString();
                            }
                            Graphics g = Graphics.FromImage(mapBmp);
                            g.DrawImage(current.bitmap, gridCoordX, gridCoordY, (float)bitmapDimX, (float)bitmapDimY);
                            g.Dispose();
                            panelMap.Refresh();
                        }
                        else if (rbQuery.Checked)
                        {
                            txtAttributes.Text = string.Empty;
                            if (rbActiveGround.Checked)
                            {
                                foreach (int a in map.groundData[mapCoordX, mapCoordY].attributes)
                                {
                                    txtAttributes.Text += a.ToString() + " ";
                                }
                            }
                            else if (rbActiveEntities.Checked)
                            {
                                foreach (int a in map.entityData[mapCoordX, mapCoordY].attributes)
                                {
                                    txtAttributes.Text += a.ToString() + " ";
                                }
                            }
                        }
                       
                        
                    }
                }
            }
        }
        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "(*.map)|*.map";
            DialogResult dr = sfd.ShowDialog();
            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                map.Save(sfd.FileName);
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "(*.map)|*.map|(*.txt)|*.txt";
            DialogResult dr = ofd.ShowDialog();
            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                map.Load(ofd.FileName);
                Init(map.width, map.height);
                txtWidth.Text = map.width.ToString();
                txtHeight.Text = map.height.ToString();
                DrawMap(true);
            }
        }
        private void DrawMap(bool showEntities)
        {
            Graphics g = Graphics.FromImage(mapBmp);
            for (int y = 0; y < map.height; y++)
            {
                for (int x = 0; x < map.width; x++)
                {

                    g.DrawImage(GetBrush(map.groundData[x, y].type).bitmap, x * 8, y * 8, (float)8, (float)8);
                    if (showEntities && !map.entityData[x, y].empty)
                    {
                        g.DrawImage(GetBrush(map.entityData[x, y].type).bitmap, x * 8, y * 8, (float)8, (float)8);
                    }
                }
            }

            panelMap.Refresh();
        }

        private void Init(int w, int h)
        {
            mapBmp = new Bitmap(w * 8, h * 8);
            Graphics g = Graphics.FromImage(mapBmp);
            g.Clear(Color.Black);
            comboBoxCellTypes.Enabled = true;
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            comboBoxCellTypes.SelectedIndex = 0;
            comboBoxCellTypes.Enabled = false;
            map.Create(int.Parse(txtWidth.Text), int.Parse(txtHeight.Text));
            Init(int.Parse(txtWidth.Text), int.Parse(txtHeight.Text));
        }

        private void btnNew_Click(object sender, EventArgs e)
        {
            map.Create(int.Parse(txtWidth.Text), int.Parse(txtHeight.Text));
            Init(int.Parse(txtWidth.Text), int.Parse(txtHeight.Text));
            Graphics g = Graphics.FromImage(mapBmp);
            g.Clear(Color.Black);
            panelMap.Refresh();
        }

        void ConvertScreenToWorld(int sx, int sy, ref int wx, ref int wy)
        {
            wx = sx / map.width;
            wy = sy / map.height;
        }

        private void panelMap_Paint(object sender, PaintEventArgs e)
        {
            if (mapBmp == null)
                return;
            e.Graphics.DrawImage(mapBmp, 0,0);
        }



        private MapBrush GetBrush(int a)
        {
            if (a == 0)
                return b0;
            else if (a == 1)
                return b1;
            else if (a == 2)
                return b2;
            else if (a == 3)
                return b3;
            else if (a == 4)
                return b4;
            else if (a == 5)
                return b5;
            else if (a == 6)
                return b6;
            else if (a == 7)
                return b7;
            else if (a == 8)
                return b8;
            else if (a == 9)
                return b9;
            else if (a == 10)
                return b10;
            else if (a == 11)
                return b11;
            else if (a == 12)
                return b12;
            else if (a == 13)
                return b13;
            else if (a == 14)
                return b14;
            else if (a == 15)
                return b15;
            else if (a == 16)
                return b16;
            else if (a == 17)
                return b17;
            else if (a == 18)
                return b18;
            else if (a == 19)
                return b19;
            else if (a == 20)
                return b20;
            else
                return b0;

        }

        private void btnFill_Click(object sender, EventArgs e)
        {
            comboBoxCellTypes_SelectedIndexChanged(null, e);
            string[] attributes = txtAttributes.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            Graphics g = Graphics.FromImage(mapBmp);
            for (int y = 0; y < map.height; y++)
            {
                for (int x = 0; x < map.width; x++)
                {
                    map.groundData[x, y].type = current.type;
                    map.groundData[x, y].attributes.Clear();
                    foreach (string a in attributes)
                    {
                        map.groundData[x, y].attributes.Add(int.Parse(a));
                    }
                    g.DrawImage(current.bitmap, x*8, y*8, (float)8, (float)8);
                }
            }

            panelMap.Refresh();
        }

        private void comboBoxCellTypes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxCellTypes.SelectedIndex != -1)
            {
                //WARNING: modify this as you add ground and entities...
                if (comboBoxCellTypes.SelectedIndex < 3)
                    rbActiveGround.Checked = true;
                else
                    rbActiveEntities.Checked = true;

                int a = comboBoxCellTypes.SelectedIndex;
                current = GetBrush(a);
                txtAttributes.Text = current.attributes;
                cbIncrementAttr.Checked = false;
            }
        }

        private void btnAttrUpdate_Click(object sender, EventArgs e)
        {
            
            string[] attributes = txtAttributes.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            if (rbActiveGround.Checked)
            {
                map.groundData[mapCoordX, mapCoordY].attributes.Clear();
                foreach (string a in attributes)
                {
                    map.groundData[mapCoordX, mapCoordY].attributes.Add(int.Parse(a));
                }
            }
            else if (rbActiveEntities.Checked)
            {
                map.entityData[mapCoordX, mapCoordY].attributes.Clear();
                foreach (string a in attributes)
                {
                    map.entityData[mapCoordX, mapCoordY].attributes.Add(int.Parse(a));
                }
            }

        }



        private void btnZeroAttr_Click(object sender, EventArgs e)
        {
            txtAttributes.Text = "0";
        }

        private void rbSet_CheckedChanged(object sender, EventArgs e)
        {
            if (rbSet.Checked)
            {
                txtAttributes.Text = current.attributes;
            }
        }

        private void cbVisibleEntities_CheckedChanged(object sender, EventArgs e)
        {
            DrawMap(cbVisibleEntities.Checked);
        }
    }
}
