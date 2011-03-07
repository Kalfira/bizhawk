﻿using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BizHawk.MultiClient
{
    public class MemoryViewer : GroupBox
    {
        public VScrollBar vScrollBar1;
        MemoryDomain Domain = new MemoryDomain("NULL", 1, Endian.Little, addr => 0, (a, v) => { });
        Font font = new Font("Courier New", 10);
        Brush regBrush = Brushes.Black;
        int RowsVisible = 0;

        const string HEADER = "       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F"; //TODO: not constant, must deal with bite size

        public MemoryViewer()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);

            this.vScrollBar1 = new VScrollBar();
            
            //TODO: based on size of MemoryViewer?
            Point n = new Point(this.Size);
            this.vScrollBar1.Location = new System.Drawing.Point(n.X-16, n.Y-this.Height);
            this.vScrollBar1.Size = new System.Drawing.Size(16, 323);
            
            this.vScrollBar1.Height = this.Height;
            this.vScrollBar1.Width = 16;
            this.vScrollBar1.Visible = true;
            this.vScrollBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                       | System.Windows.Forms.AnchorStyles.Right)));
            this.vScrollBar1.LargeChange = 16;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.TabIndex = 0;
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);

            this.Controls.Add(this.vScrollBar1);
        }

        protected unsafe override void OnPaint(PaintEventArgs e)
        {
            unchecked
            {
                int row = 0;
                int rowX = 8;
                int rowY = 16;
                int rowYoffset = 20;
                string rowStr;

                e.Graphics.DrawString(HEADER, font, regBrush, new Point(rowX, rowY));
                e.Graphics.DrawLine(new Pen(regBrush), this.Left + 38, this.Top, this.Left + 38, this.Bottom - 40);
                e.Graphics.DrawLine(new Pen(regBrush), this.Left, 34, this.Right - 16, 34);

                for (int i = 0; i < RowsVisible; i++)
                {
                    row = i + vScrollBar1.Value;
                    rowStr = String.Format("{0:X4}", row * 16) + "  "; //TODO: num digits based on size of domain
                    for (int j = 0; j < 16; j++)
                    {
                        rowStr += String.Format("{0:X2}", Domain.PeekByte((row * 16) + j)) + " "; //TODO: format based on data size
                    }

                    e.Graphics.DrawString(rowStr, font, regBrush, new Point(rowX, (rowY * (i + 1)) + rowYoffset));
                }
            }
        }

        public void ResetScrollBar()
        {
            vScrollBar1.Value = 0;
            Refresh();
        }

        public void SetUpScrollBar()
        {
            RowsVisible = this.Height / 16;
            int totalRows = Domain.Size / 16;
            int MaxRows = (totalRows - RowsVisible) + 17;

            if (MaxRows > 0)
            {
                vScrollBar1.Visible = true;
                if (vScrollBar1.Value > MaxRows)
                    vScrollBar1.Value = MaxRows;
                vScrollBar1.Maximum = MaxRows;
            }
            else
                vScrollBar1.Visible = false;

        }

        public void SetMemoryDomain(MemoryDomain d)
        {
            Domain = d;
            SetUpScrollBar();
            vScrollBar1.Value = 0;
            Refresh();
        }

        public string GetMemoryDomainStr()
        {
            return Domain.ToString();
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            this.SetUpScrollBar();
            this.Refresh();
        }
    }
}
