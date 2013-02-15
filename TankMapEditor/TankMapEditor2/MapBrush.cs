using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace TankMapEditor2
{
    public class MapBrush
    {
        public Bitmap bitmap = null;
        public int type;
        public string attributes = string.Empty;

        public MapBrush(string f, int t, string a)
        {
            bitmap = new Bitmap(f);
            type = t;
            attributes = a;
        }
    }
}
