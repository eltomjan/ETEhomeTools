using MultiConvert.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace MultiConvert
{
    public partial class Form1 : Form
    {
        private FormLogic model;
        private MemoryStream mem;
        private StringBuilder res;
        const int PreviewLimit = 10000;
        bool? CSV = null;
        public Form1()
        {
            InitializeComponent();
            model = new FormLogic();
        }

        private void LoadJSON_Click(object sender, EventArgs e)
        {
            if (openJsonDialog1.ShowDialog() == DialogResult.OK)
            {
                model.LoadJSON(openJsonDialog1.FileName);
                Byte[] preview = model.JSONdata;
                if(null != preview)
                {
                    JsonPreviewBox.Text = model.JSON.ToString();
                }
            }
        }

        private void Convert2CSV_Click(object sender, EventArgs e)
        {
            CSV = true;
            Save.Text = "Save CSV";
/*
            res = model.Convert2CSVvisual(Horizontal.Checked, SortLast.Checked, ID.Text);
            if (res.Length < PreviewLimit) Result.Text = res.ToString();
            else Result.Text = res.ToString(0, PreviewLimit) + "...";
            return;//*/
            mem = new MemoryStream();
            StreamWriter sw = new StreamWriter(mem);
            model.Convert2CSVquick(sw, Horizontal.Checked, SortLast.Checked, ID.Text);
            if (mem.Length < PreviewLimit) Result.Text = Encoding.UTF8.GetString(mem.ToArray(), 0, (int)mem.Length);
            else Result.Text = Encoding.UTF8.GetString(mem.ToArray(), 0, PreviewLimit) + "...";
        }

        private void Convert2XML_Click(object sender, EventArgs e)
        {
            mem = null;
            CSV = false;
            Save.Text = "Save XML";
            res = model.Convert2XML();
            if (res.Length < PreviewLimit) Result.Text = res.ToString();
            else Result.Text = res.ToString(0, PreviewLimit) + "...";
        }

        private void Save_Click(object sender, EventArgs e)
        {
            if (null == CSV) return;
            String ext = (bool)CSV ? ".csv" : ".xml";
            if (mem != null && mem.Length > 0) {
                model.Save(openJsonDialog1.FileName, ext, mem);
            } else if (res != null && res.Length > 0) {
                model.Save(openJsonDialog1.FileName, ext, res);
            }
        }
    }
}
