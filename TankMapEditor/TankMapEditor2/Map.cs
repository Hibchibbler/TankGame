using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TankMapEditor2
{
    public class Cell
    {
        public Cell()
        {
            attributes = new List<int>();
            empty = true;
        }

        public int type;
        public bool empty;
        public List<int> attributes;
    }

    public class Map
    {
        public int width;
        public int height;
        public Cell[,] groundData;
        public Cell[,] entityData;

        public Map()
        {
        }

        public void Create(int width, int height)
        {
            this.width = width;
            this.height = height;
            groundData = new Cell[width,height];
            entityData = new Cell[width, height];
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    groundData[x, y] = new Cell();
                    entityData[x, y] = new Cell();
                }
            }

                
        }
        public void Load(string filename)
        {
            System.IO.BinaryReader br = new System.IO.BinaryReader(System.IO.File.Open(filename, System.IO.FileMode.Open));

            width = br.ReadInt32();
            height = br.ReadInt32();

            Create(width, height);
            
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int attrCnt = 0;
                    
                    groundData[x, y].type = br.ReadInt32();
                    attrCnt = br.ReadInt32();
                    for (int k = 0; k < attrCnt; k++)
                    {
                        int attr;
                        attr = br.ReadInt32();
                        groundData[x, y].attributes.Add(attr);
                    }
                }
            }

            int entityCnt = 0;
            entityCnt = br.ReadInt32();
            for (int e = 0; e < entityCnt; e++)
            {
                Cell entityCell = new Cell();
                int x, y, attrCnt;
                entityCell.type = br.ReadInt32();
                entityCell.empty = false;
                x = br.ReadInt32();
                y = br.ReadInt32();

                attrCnt = br.ReadInt32();
                for (int k = 0; k < attrCnt; k++)
                {
                    entityCell.attributes.Add(br.ReadInt32());
                }


                entityData[x, y] = entityCell;
            }
            br.Close();
        }

        public void Save(string filename)
        {
            System.IO.BinaryWriter bw = new System.IO.BinaryWriter(System.IO.File.Open(filename, System.IO.FileMode.Create));

            bw.Write(width);
            bw.Write(height);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    bw.Write(groundData[x,y].type);
                    bw.Write(groundData[x, y].attributes.Count);
                    for (int k = 0; k < groundData[x, y].attributes.Count; k++)
                    {
                        bw.Write(groundData[x, y].attributes[k]);
                    }
                }
            }

            int entityCnt = 0;
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                    if (!entityData[x, y].empty)
                        entityCnt++;

            bw.Write(entityCnt);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (!entityData[x, y].empty)
                    {
                        bw.Write(entityData[x, y].type);
                        bw.Write(x);
                        bw.Write(y);
                        bw.Write(entityData[x, y].attributes.Count);
                        for (int k = 0; k < entityData[x, y].attributes.Count; k++)
                        {
                            bw.Write(entityData[x, y].attributes[k]);
                        }
                    }
                }
            }


            bw.Close();
        }
    }
}
