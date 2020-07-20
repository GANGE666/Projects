package com.whuctf.mobile.record_number;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;


import jxl.Sheet;
import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;

/*

public class MainActivity extends AppCompatActivity implements View.OnClickListener,SensorEventListener {
    private SensorManager sensorManager;
    private String excelPath;//文件存储路径
    private TextView modeText;
    private TextView showText1;
    private TextView showText2;
    private TextView showtimes;
    private TextView excelLoc;
    private Button btn_start;
    private Button btn_end;
    private Button changemod;
    EditText inputfilename;//文件名输入框
    private boolean processState = false;   //标记当前是否已经在记录
    private int times=0;

    Timer timer=null;
    ArrayList<Integer> SeqList=new ArrayList<Integer>();//序号
    ArrayList <Float> AccList=new ArrayList<Float>();//加速度
    ArrayList <Float> GyrList=new ArrayList<Float>();//陀螺仪
    private float AccData[]=new float[3];//陀螺仪
    private float GyrData[]=new float[3];//加速度


    //CreateXls data_XLS=new CreateXls();//需要导入工程或者jxl包

    private int mode=2;
    public static final int SENSOR_RATE_NORMAL=20;
    public static final int SENSOR_RATE_UI=10;
    public static final int SENSOR_RATE_GAME=5;
    public static final int SENSOR_RATE_FASTEST=0;
    private int hzmode[]= {SENSOR_RATE_FASTEST,SENSOR_RATE_GAME,SENSOR_RATE_UI,SENSOR_RATE_NORMAL};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        modeText = (TextView) findViewById(R.id.modeText);
        showText1 = (TextView) findViewById(R.id.showText1);
        showText2 = (TextView) findViewById(R.id.showText2);
        showtimes = (TextView) findViewById(R.id.showtimes);
        inputfilename=(EditText)findViewById(R.id.filename);
        excelLoc=(TextView)findViewById(R.id.excellocation);
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        bindViews();
        displaymode();
    }

    private void bindViews() {
        btn_start = (Button)findViewById(R.id.btn_start);
        btn_start.setOnClickListener(new ButtonListener());
        btn_end = (Button)findViewById(R.id.btn_end);
        btn_end.setOnClickListener(new ButtonListener());
        changemod = (Button)findViewById(R.id.changemod);
        changemod.setOnClickListener(new ButtonListener());
        btn_end.setEnabled(false);//关闭按钮失效
    }



    @Override
    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this,
                sensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION),
                SensorManager.SENSOR_DELAY_FASTEST);
        sensorManager.registerListener(this,
                sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE),
                SensorManager.SENSOR_DELAY_FASTEST);
    }

    @Override
    protected void onStop() {
        super.onStop();
        sensorManager.unregisterListener(this);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        int type = event.sensor.getType();
        StringBuilder sb;
        float[] values = event.values;
        if(processState) {
            switch(type) {
                case Sensor.TYPE_LINEAR_ACCELERATION:
                    sb = new StringBuilder();
                    sb.append("线性加速度传感器:\n");
                    sb.append("X:" + values[0] + "\n");
                    sb.append("Y:" + values[1] + "\n");
                    sb.append("Z:" + values[2] + "\n");
                    //AccList.add(values[0]);AccList.add(values[1]);AccList.add(values[2]);
                    AccData[0]=values[0];AccData[1]=values[1];AccData[2]=values[2];
                    showText1.setText(sb.toString());
                    break;
                case Sensor.TYPE_GYROSCOPE:
                    sb = new StringBuilder();
                    sb.append("陀螺仪传感器:\n");
                    sb.append("X:" + values[0] + "\n");
                    sb.append("Y:" + values[1] + "\n");
                    sb.append("Z:" + values[2] + "\n");
                    showText2.setText(sb.toString());
                    //GyrList.add(values[0]);GyrList.add(values[1]);GyrList.add(values[2]);
                    GyrData[0]=values[0]; GyrData[1]=values[1]; GyrData[2]=values[2];
                    break;
                default:
                    break;
            }
        }
    }


    private class ButtonListener implements View.OnClickListener {
        public void onClick(View v) {
            // TODO 自动生成的方法存根
            switch (v.getId()) {
                case R.id.btn_start:
                    if (processState == false) {
                        if (inputfilename.getText().toString().equals(""))
                        {
                            onStop();//传感器失效
                            Toast.makeText(MainActivity.this, "文件名不能为空，请输入", Toast.LENGTH_SHORT).show();
                            break;
                        }
                        Toast.makeText(MainActivity.this, "开始记录", Toast.LENGTH_SHORT).show();
                        times=0;
                        processState = true;
                        inputfilename.setEnabled(false);//数据一旦开始采集，不允许输入文件名
                        btn_end.setEnabled(true);//关闭按钮启用
                        btn_start.setEnabled(false);//开始按钮失效
                        changemod.setEnabled(false);//模式按钮失效
                    }

                    displaymode();



                    // Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show();
                    // excelPath=data_XLS.getExcelDir()+ File.separator+
                    //        inputfilename.getText().toString()+".xls";
                    //data_XLS.excelCreate(new File(excelPath));//在这里创建
                    //excelLoc.setText("excel文件保存在:\n"+excelPath);
                    //excelLoc.setText("dir是 "+data_XLS.getExcelDir()+"\n"+"File.separator是 "+ File.separator+"\n");
                    onStop();
                    onResume();
                    if(timer==null) {timer=new Timer();}
                    timer.schedule(new TimerTask()
                    {
                        @Override
                        public void run() {
                            times++;
                            SeqList.add(times);
                            AccList.add(AccData[0]);AccList.add(AccData[1]);AccList.add(AccData[2]);
                            GyrList.add(GyrData[0]);GyrList.add(GyrData[1]);GyrList.add(GyrData[2]);
                        }
                    },0,hzmode[mode]);
                    break;

                case R.id.btn_end:
                    if (processState == true) {
                        timer.cancel();
                        timer=null;
                        showtimes.setText("记录次数为"+ times);
                        processState = false;


                        CreateXls data_XLS=new CreateXls();//需要导入工程或者jxl包
                        Calendar c = Calendar.getInstance();//可以对每个时间域单独修改

                        int hour = c.get(Calendar.HOUR_OF_DAY);
                        int minute = c.get(Calendar.MINUTE);
                        int second = c.get(Calendar.SECOND);


                        //File.separator是文件里面的斜杠的意思
                        excelPath=data_XLS.getExcelDir()+ File.separator+inputfilename.getText().toString()+File.separator+
                                inputfilename.getText().toString()+"-"+String.valueOf(hour)+"-"+String.valueOf(minute)+"-"+String.valueOf(second)+".xls";

                        data_XLS.makedir(inputfilename.getText().toString());//针对数字的文件夹
                        data_XLS.excelCreate(new File(excelPath));
                        excelLoc.setText("excel文件保存在:\n"+excelPath);

                        WriteXls(AccList,GyrList,SeqList);//核心代码：将采集的数据写入文件中
                        SeqList.clear();
                        AccList.clear();//清除链表数据
                        GyrList.clear();
                        times=0;
                        onStop();
                        inputfilename.setEnabled(true);//数据一旦结束收集，允许输入文件名
                        btn_end.setEnabled(false);//关闭按钮失效
                        btn_start.setEnabled(true);//开始按钮启用
                        changemod.setEnabled(true);//模式按钮启用
                        Toast.makeText(MainActivity.this, "完成记录", Toast.LENGTH_SHORT).show();
                    }
                    break;

                default:
                    break;

            }

        }

    }

    public void displaymode() {
        switch (hzmode[mode]) {
            case SENSOR_RATE_FASTEST:
                modeText.setText("延迟 "+ String.valueOf(SENSOR_RATE_FASTEST) + "ms");
                break;
            case SENSOR_RATE_GAME:
                modeText.setText("延迟 "+ String.valueOf(SENSOR_RATE_GAME) + "ms");
                break;
            case SENSOR_RATE_NORMAL:
                modeText.setText("延迟 "+ String.valueOf(SENSOR_RATE_NORMAL) + "ms");
                break;
            case SENSOR_RATE_UI:
                modeText.setText("延迟 "+ String.valueOf(SENSOR_RATE_UI) + "ms");
                break;
            default:
                break;
        }
    }

    @Override
    public void onClick(View arg0) {
        // TODO 自动生成的方法存根

    }

    public void WriteXls(ArrayList<Float> accdata,ArrayList<Float> gyrdata,ArrayList<Integer> SeqList)
    {
        Workbook wb;
        WritableWorkbook wbook;//需要导入jxl工程或者包
        WritableSheet sh;
        Sheet sheet;
        try {
            wb=Workbook.getWorkbook(new File(excelPath));//获取原始文档
            sheet=wb.getSheet(0);//得到一个工作对象
            wbook=Workbook.createWorkbook(new File(excelPath),wb);//根据book创建一个操作对象
            sh=wbook.getSheet(0);//得到一个工作


            for(int i=0,seqrow=1;i<SeqList.size();i++)
            {
                if  (SeqList!=null && SeqList.get(i)!=null)
                {
                    Label label=new Label(0,seqrow,String.valueOf(SeqList.get(i)));
                    sh.addCell(label);
                    seqrow++;
                }
            }

            //逐个写入加速度数据到文件中去！
            for(int i=0,acc_Row=1;i<accdata.size();)
            {
                if  (accdata!=null && accdata.get(i)!=null)
                {
                    for(int j=1;j<4;j++)
                    {
                        Label label=new Label(j,acc_Row,String.valueOf(accdata.get(i)));
                        sh.addCell(label);
                        i++;
                    }
                    acc_Row++;
                }
            }
            //逐个写入陀螺数据到文件中去！
            for(int i=0,gyr_Row=1;i<gyrdata.size();)
            {
                if  (gyrdata!=null && gyrdata.get(i)!=null)
                {
                    for(int j=4;j<7;j++)
                    {
                        Label label=new Label(j,gyr_Row,String.valueOf(gyrdata.get(i)));
                        sh.addCell(label);
                        i++;
                    }
                    gyr_Row++;
                }
            }
            //写入数据
            wbook.write();
            wbook.close();
        } catch (Exception e2){
            System.out.print(e2.toString()+"--");
            System.out.print("--异常--");
        }
    }

}

*/