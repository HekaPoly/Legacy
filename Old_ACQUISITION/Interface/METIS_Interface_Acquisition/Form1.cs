using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Collections;

namespace METIS_FormV2
{
    public partial class Form1 : Form
    {
        #region Variables
        string lineDataIn;
        byte[] commandToSend = { 2 };
        int index, serialDataIn1, serialDataIn2, indexOfStop, indexOfEnd, numEssai;
        int[] dataArray1 = new int[200];
        int[] dataArray2 = new int[200];

        //For writing .txt file 
        ArrayList fileArray1 = new ArrayList(); //First position of array is muscle_1 - Next is muscle_2 and so on
        #endregion


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            btnOpenPort.Enabled = true;
            btnStart.Enabled = false;
            btnStop.Enabled = false;
            cbBaudRate.Text = "115200";

            chartDataM1.Series["Muscle1"].Points.AddXY(1, 1);
            chartDataM1.Series["Muscle2"].Points.AddXY(1, 1);

            chartDataM1.ChartAreas[0].AxisX.Maximum = 200;
            chartDataM1.ChartAreas[0].AxisX.Minimum = 0;
            chartDataM1.ChartAreas[0].AxisY.Maximum = 4096;
            chartDataM1.ChartAreas[0].AxisY.Minimum = 0;
        }

        #region Buttons
        private void btnOpenPort_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = cbComPorts.Text;
                serialPort1.BaudRate = Convert.ToInt32(cbBaudRate.Text);

                btnOpenPort.Enabled = false;
                btnStart.Enabled = true;
                btnStop.Enabled = false;

                chartDataM1.Series["Muscle1"].Points.Clear();
                chartDataM1.Series["Muscle2"].Points.Clear();

            }
            catch(Exception error)
            {
                MessageBox.Show(error.Message);
            }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == false)
            {
                serialPort1.Open();
            }

            btnStart.Enabled = false;
            btnStop.Enabled = true;
            cbComPorts.Enabled = false;
            cbBaudRate.Enabled = false;

            numEssai++;
            
            commandToSend[0] = 1;
            serialPort1.Write(commandToSend, 0, 1);
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {
                btnStart.Enabled = true;
                btnStop.Enabled = false;

                commandToSend[0] = 2;
                serialPort1.Write(commandToSend, 0, 1);

                //Write .txt file 
                string fileName = String.Format("Acquisition-{0}.txt", numEssai);
                StreamWriter file = new StreamWriter(@fileName);

                for (int i = 0; i < fileArray1.Count-1; i++) {
                    file.WriteLine("1 {0} | 2 {1}", fileArray1[i], fileArray1[i + 1]);
                }

                file.Close();
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message);
            }
        }
        #endregion

        #region ComboBox
        private void cbComPorts_DropDown(object sender, EventArgs e)
        {
            string[] portsList = SerialPort.GetPortNames();
            cbComPorts.Items.Clear();
            cbComPorts.Items.AddRange(portsList);
        }
        #endregion

        #region Data Reception
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //Read line printed by Teensy after converting a value
            lineDataIn = serialPort1.ReadLine();

            //Process line
            indexOfStop = lineDataIn.IndexOf('%');
            indexOfEnd = lineDataIn.IndexOf('&');

            serialDataIn1 = Convert.ToInt32(lineDataIn.Substring(0, indexOfStop));
            serialDataIn2 = Convert.ToInt32(lineDataIn.Substring(indexOfStop + 1, indexOfEnd - (indexOfStop + 1)));

            fileArray1.Add(serialDataIn1);
            fileArray1.Add(serialDataIn2);

            this.BeginInvoke(new EventHandler(UpdateArray));
        }

        private void UpdateArray(object sender, EventArgs e)
        {
            try
            {
                if (index < 1)
                {
                    chartDataM1.Series["Muscle1"].Points.AddY(serialDataIn1);
                    chartDataM1.Series["Muscle2"].Points.AddY(serialDataIn2);

                    chartDataM1.Series["Muscle1"].Points.Clear();
                    chartDataM1.Series["Muscle2"].Points.Clear();

                    index++;
                }
                else
                {
                    ShiftData(dataArray1, serialDataIn1);
                    ShiftData(dataArray2, serialDataIn2);

                    chartDataM1.Series["Muscle1"].Points.Clear();
                    chartDataM1.Series["Muscle2"].Points.Clear();

                    for (int i = 0; i < dataArray1.Length - 1; i++)
                    {
                        chartDataM1.Series["Muscle1"].Points.AddY(dataArray1[i]);
                        chartDataM1.Series["Muscle2"].Points.AddY(dataArray2[i]);
                    }
                }
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message);
            }
        }

        private void ShiftData(int[] array, int newValue)
        {
            for (int i = 1; i < array.Length; i++)
            {
                array[i - 1] = array[i];
                array[array.Length - 1] = newValue;
            }
            
        }

        #endregion
    }
}
