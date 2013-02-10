using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TankMapMaker
{
    public class MapCell
    {
        public MapCell()
        {
            attributes = new List<int>();
        }
        public int floorCode;
        public List<int> attributes;
    }

    public class Map
    {
        public void Init(int w, int h)
        {
            width = w;
            height = h;
            mapData = new MapCell[w, h];
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    mapData[x, y] = new MapCell();
                }
            }
        }

        public int width;
        public int height;
        public MapCell[,] mapData;
    }
}
