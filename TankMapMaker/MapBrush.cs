using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TankMapMaker
{
    public class MapBrush
    {
        public MapBrush(string b, int c)
        {
            bitmap = new Bitmap(b);
            code = c;
        }
        public Bitmap bitmap;
        public int code;
    }
}
