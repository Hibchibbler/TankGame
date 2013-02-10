using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TankMapMaker
{
    public partial class Form1 : Form
    {
        int mapCoordX;//Last Clicked Spot
        int mapCoordY;//        "

        Map map = new Map();

        MapBrush current = null;
        MapBrush zero = new MapBrush("wall.png",0);//wall
        MapBrush one = new MapBrush("floor1.png",1);//floor 1
        MapBrush two = new MapBrush("floor2.png",2);//floor 2


        MapBrush three = new MapBrush("base1.png",3);// team 1 garage
        MapBrush four = new MapBrush("base2.png", 4);//team 2 garage
        MapBrush five = new MapBrush("turret1.png", 5);//team 1 minion generator
        MapBrush six = new MapBrush("turret2.png", 6);//team 2 minion generator

        MapBrush b7 = new MapBrush("w1_1.png", 7);//team 1 minion 1 waypoint
        MapBrush b8 = new MapBrush("w2_1.png", 8);//team 1 minion 2 waypoint
        MapBrush b9 = new MapBrush("w3_1.png", 9);//team 1 minion 3 waypoint

        MapBrush b10 = new MapBrush("w1_2.png", 10);//team 2 minion 1 waypoint
        MapBrush b11 = new MapBrush("w2_2.png", 11);//team 2 minion 2 waypoint
        MapBrush b12 = new MapBrush("w3_2.png", 12);//team 2 minion 3 waypoint


        Bitmap mapBmp = null;//new Bitmap(width * 16, height * 16);

        public Form1()
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

                //Convert Grid to Map
                mapCoordX = gridCoordX / (int)bitmapDimX;
                mapCoordY = gridCoordY / (int)bitmapDimY;

                //Print out Coordinate Info
                lblMouseCoordinates.Text = String.Format("Mouse: {0},{1}", e.X, e.Y);
                lblGridCoordinates.Text = String.Format("Grid : {0},{1}", gridCoordX, gridCoordY);
                lblMapCoordinates.Text = String.Format("Map   : {0},{1}", mapCoordX, mapCoordY);

                //Check for out of bounds

                if (mapCoordX >= 0 && mapCoordX < map.width)
                {
                    if (mapCoordY >= 0 && mapCoordY < map.height)
                    {
                        
                        txtAttributes.Text = string.Empty;
                        foreach (int a in map.mapData[mapCoordX, mapCoordY].attributes)
                        {
                            txtAttributes.Text += a.ToString() + ", ";
                        }
 
                        map.mapData[mapCoordX, mapCoordY].floorCode = current.code;
                        Graphics g = Graphics.FromImage(mapBmp);

                        g.DrawImage(current.bitmap, gridCoordX, gridCoordY, (float)bitmapDimX, (float)bitmapDimY);
                        g.Dispose();
                        panelMap.Refresh();
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
                System.IO.BinaryWriter bw = new System.IO.BinaryWriter(System.IO.File.Open(sfd.FileName, System.IO.FileMode.Create));


                bw.Write(map.width);//columns
                bw.Write(map.height);//rows
                for (int y = 0; y < map.height; y++)
                {
                    for (int x = 0; x < map.width; x++)
                    {
                        bw.Write(map.mapData[x, y].floorCode);
                        bw.Write(map.mapData[x, y].attributes.Count);
                        foreach (int a in map.mapData[x, y].attributes)
                        {
                            bw.Write(a);
                        }
                    }
                }
                bw.Close();
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "(*.map)|*.map|(*.txt)|*.txt";
            DialogResult dr = ofd.ShowDialog();
            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                System.IO.BinaryReader br = new System.IO.BinaryReader(System.IO.File.Open(ofd.FileName, System.IO.FileMode.Open));


                int width = br.ReadInt32();//columns
                int height = br.ReadInt32();//rows
                txtWidth.Text = width.ToString();
                txtHeight.Text = height.ToString();

                Init(width, height);

                Graphics g = Graphics.FromImage(mapBmp);

                for (int y = 0; y < map.height; y++)
                {
                    for (int x = 0; x < map.width; x++)
                    {
                        //br.Write(map.mapData[x, y].floorCode);
                        map.mapData[x, y].floorCode = br.ReadInt32();
                        map.mapData[x, y].attributes.Clear();
                        int numAttr = br.ReadInt32();
                        for (int a = 0; a < numAttr; a++)
                        {
                            int attr = br.ReadInt32();
                            map.mapData[x, y].attributes.Add(attr);
                        }
                        g.DrawImage(GetBrush(map.mapData[x, y].floorCode).bitmap, x * 8, y * 8, (float)8, (float)8);
                    }
                }
                panelMap.Refresh();
                br.Close();

            }
        }

        private void Init(int w, int h)
        {
            map.Init(w, h);
            mapBmp = new Bitmap(w * 8, h * 8);
            Graphics g = Graphics.FromImage(mapBmp);
            g.Clear(Color.Black);
            comboBoxCellTypes.Enabled = true;
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            comboBoxCellTypes.SelectedIndex = 0;
            comboBoxCellTypes.Enabled = false;

            Init(int.Parse(txtWidth.Text), int.Parse(txtHeight.Text));
        }
        private void btnSet_Click(object sender, EventArgs e)
        {
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
                return zero;
            else if (a == 1)
                return one;
            else if (a == 2)
                return two;
            else if (a == 3)
                return three;
            else if (a == 4)
                return four;
            else if (a == 5)
                return five;
            else if (a == 6)
                return six;
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
            else
                return zero;

        }

        private void btnFill_Click(object sender, EventArgs e)
        {
            if (comboBoxCellTypes.SelectedIndex != -1)
            {
                int a = comboBoxCellTypes.SelectedIndex;
                current = GetBrush(a);
            }
            else
                current = zero;
            
            Graphics g = Graphics.FromImage(mapBmp);
            for (int y = 0; y < map.height; y++)            
            {
                for (int x = 0; x < map.width; x++)
                {
                    map.mapData[x, y].floorCode = current.code;
                    g.DrawImage(current.bitmap, x*8, y*8, (float)8, (float)8);
                }
            }

            panelMap.Refresh();
        }

        private void panelMap_MouseDown(object sender, MouseEventArgs e)
        {
            panelMap_MouseMove(sender, e);
        }

        private void comboBoxCellTypes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxCellTypes.SelectedIndex != -1)
            {
                int a = comboBoxCellTypes.SelectedIndex;
                current = GetBrush(a);
            }
        }

        private void btnAttrUpdate_Click(object sender, EventArgs e)
        {
            string[] attributes = txtAttributes.Text.Split(new char[] { ',', ' ' }, StringSplitOptions.RemoveEmptyEntries);
            map.mapData[mapCoordX, mapCoordY].attributes.Clear();
            foreach (string a in attributes)
            {
                map.mapData[mapCoordX, mapCoordY].attributes.Add(int.Parse(a));
            }
        }
    }
}
