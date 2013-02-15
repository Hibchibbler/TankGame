using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TankMapEditor2
{
    public class DoubleBufferPanel : System.Windows.Forms.Panel
    {
        public DoubleBufferPanel()
        {
            this.DoubleBuffered = true;
            this.ResizeRedraw = true;
        }
    }
}
