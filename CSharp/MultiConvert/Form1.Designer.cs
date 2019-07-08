namespace MultiConvert
{
    partial class Form1
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
            this.LoadJSON = new System.Windows.Forms.Button();
            this.openJsonDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.JsonPreviewBox = new System.Windows.Forms.RichTextBox();
            this.Convert2CSV = new System.Windows.Forms.Button();
            this.Horizontal = new System.Windows.Forms.CheckBox();
            this.SortLast = new System.Windows.Forms.CheckBox();
            this.Result = new System.Windows.Forms.RichTextBox();
            this.ID = new System.Windows.Forms.TextBox();
            this.rows = new System.Windows.Forms.Label();
            this.Convert2XML = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // LoadJSON
            // 
            this.LoadJSON.Location = new System.Drawing.Point(13, 13);
            this.LoadJSON.Name = "LoadJSON";
            this.LoadJSON.Size = new System.Drawing.Size(85, 23);
            this.LoadJSON.TabIndex = 0;
            this.LoadJSON.Text = "Load JSON";
            this.LoadJSON.UseVisualStyleBackColor = true;
            this.LoadJSON.Click += new System.EventHandler(this.LoadJSON_Click);
            // 
            // openJsonDialog1
            // 
            this.openJsonDialog1.DefaultExt = "json";
            this.openJsonDialog1.Filter = "JSON files|*.json";
            // 
            // JsonPreviewBox
            // 
            this.JsonPreviewBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.JsonPreviewBox.Location = new System.Drawing.Point(0, 305);
            this.JsonPreviewBox.Name = "JsonPreviewBox";
            this.JsonPreviewBox.Size = new System.Drawing.Size(800, 145);
            this.JsonPreviewBox.TabIndex = 1;
            this.JsonPreviewBox.Text = "";
            // 
            // Convert2CSV
            // 
            this.Convert2CSV.Location = new System.Drawing.Point(12, 71);
            this.Convert2CSV.Name = "Convert2CSV";
            this.Convert2CSV.Size = new System.Drawing.Size(86, 23);
            this.Convert2CSV.TabIndex = 2;
            this.Convert2CSV.Text = "Convert 2 CSV";
            this.Convert2CSV.UseVisualStyleBackColor = true;
            this.Convert2CSV.Click += new System.EventHandler(this.Convert2CSV_Click);
            // 
            // Horizontal
            // 
            this.Horizontal.AutoSize = true;
            this.Horizontal.Location = new System.Drawing.Point(20, 100);
            this.Horizontal.Name = "Horizontal";
            this.Horizontal.Size = new System.Drawing.Size(73, 17);
            this.Horizontal.TabIndex = 3;
            this.Horizontal.Text = "Horizontal";
            this.Horizontal.UseVisualStyleBackColor = true;
            // 
            // SortLast
            // 
            this.SortLast.AutoSize = true;
            this.SortLast.Location = new System.Drawing.Point(20, 123);
            this.SortLast.Name = "SortLast";
            this.SortLast.Size = new System.Drawing.Size(59, 17);
            this.SortLast.TabIndex = 4;
            this.SortLast.Text = "Sort All";
            this.SortLast.UseVisualStyleBackColor = true;
            // 
            // Result
            // 
            this.Result.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Result.Location = new System.Drawing.Point(104, 13);
            this.Result.Name = "Result";
            this.Result.Size = new System.Drawing.Size(696, 294);
            this.Result.TabIndex = 5;
            this.Result.Text = "";
            // 
            // ID
            // 
            this.ID.Location = new System.Drawing.Point(13, 146);
            this.ID.Name = "ID";
            this.ID.Size = new System.Drawing.Size(86, 20);
            this.ID.TabIndex = 6;
            // 
            // rows
            // 
            this.rows.AutoSize = true;
            this.rows.Location = new System.Drawing.Point(12, 176);
            this.rows.MinimumSize = new System.Drawing.Size(86, 0);
            this.rows.Name = "rows";
            this.rows.Size = new System.Drawing.Size(86, 13);
            this.rows.TabIndex = 7;
            // 
            // Convert2XML
            // 
            this.Convert2XML.Location = new System.Drawing.Point(12, 42);
            this.Convert2XML.Name = "Convert2XML";
            this.Convert2XML.Size = new System.Drawing.Size(86, 23);
            this.Convert2XML.TabIndex = 8;
            this.Convert2XML.Text = "Convert 2 XML";
            this.Convert2XML.UseVisualStyleBackColor = true;
            this.Convert2XML.Click += new System.EventHandler(this.Convert2XML_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(12, 192);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(86, 23);
            this.Save.TabIndex = 9;
            this.Save.Text = "Save";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.Convert2XML);
            this.Controls.Add(this.rows);
            this.Controls.Add(this.ID);
            this.Controls.Add(this.Result);
            this.Controls.Add(this.SortLast);
            this.Controls.Add(this.Horizontal);
            this.Controls.Add(this.Convert2CSV);
            this.Controls.Add(this.JsonPreviewBox);
            this.Controls.Add(this.LoadJSON);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button LoadJSON;
        private System.Windows.Forms.OpenFileDialog openJsonDialog1;
        private System.Windows.Forms.RichTextBox JsonPreviewBox;
        private System.Windows.Forms.Button Convert2CSV;
        private System.Windows.Forms.CheckBox Horizontal;
        private System.Windows.Forms.RichTextBox Result;
        private System.Windows.Forms.CheckBox SortLast;
        private System.Windows.Forms.TextBox ID;
        private System.Windows.Forms.Label rows;
        private System.Windows.Forms.Button Convert2XML;
        private System.Windows.Forms.Button Save;
    }
}

