/*
package com.whuctf.mobile.record_number;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;


import jxl.Sheet;
import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;

import static java.lang.Thread.sleep;


public class MainActivity_2 extends AppCompatActivity implements View.OnClickListener,SensorEventListener {
    private SensorManager sensorManager;
    private String excelPath;//文件存储路径
    private TextView modeText;
    private TextView showText1;
    private TextView showText2;
    private TextView showtimes;
    private TextView excelLoc;
    private TextView f_path;
    private TextView network_delay;
    private Button btn_start;
    private Button btn_end;
    private Button btn_connect_server;

    private ImageButton btn_go;
    private ImageButton btn_save;
    EditText inputfilename;//文件名输入框
    EditText inputIpConfig;
    private boolean processState = false;   //标记当前是否已经在记录
    private int times=0;

    Timer timer=null;
    ArrayList<Integer> SeqList=new ArrayList<Integer>();//序号
    ArrayList<Long> TimeList=new ArrayList<Long>();//时间戳
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

    private int timer_s = hzmode[mode]; //timer采样频率


    //socket相关
    private String server_address = null;
    private int server_port = 12345;
    private Socket sk;
    private InputStream is;
    private OutputStream os;
    private String Send_file_name = null;
    Map<String, byte[]> map = null;

    private long time_bias = 0; //服务器与手机端时间戳差值，通过延迟来估计


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_2);
        modeText = (TextView) findViewById(R.id.modeText);
        showText1 = (TextView) findViewById(R.id.showText1);
        showText2 = (TextView) findViewById(R.id.showText2);
        showtimes = (TextView) findViewById(R.id.showtimes);
        inputfilename=(EditText)findViewById(R.id.filename);
        inputIpConfig=(EditText)findViewById(R.id.IP_Config);
        excelLoc=(TextView)findViewById(R.id.excellocation);
        f_path = (TextView)findViewById(R.id.path);
        network_delay = (TextView)findViewById(R.id.network_delay);
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        bindViews();
        displaymode();

    }

    public static byte[] intToByteArray(int a){
        return new byte[]{
                (byte)((a>>24)&0xFF),
                (byte)((a>>16)&0xFF),
                (byte)((a>>8)&0xFF),
                (byte)((a)&0xFF),
        };
    }
    public static int byteArrayToInt(byte[] b){
        return b[3]&0xFF|
                (b[2]&0xFF)<<8|
                (b[1]&0xFF)<<16|
                (b[0]&0xFF)<<24;
    }

    public static int byteArratToInt_8(byte[] b){
        return b[0]&0xFF;
    }

    //从socket中读取数据
    private Map<String, byte[]> read_data_from_socket(){
        Map<String, byte[]> map = new HashMap<String, byte[]>();
        try {
            is = sk.getInputStream();
            byte buffer[] = new byte[1024];
            int len = 0;
            while ((len = is.read(buffer)) != -1) {
                //String buf = new String(buffer, 0, len);
                break;
            }
            map.put("data",buffer);
            map.put("len", intToByteArray(len));
            return map;
        }catch (Exception e){
            e.printStackTrace();
            return map;
        }
    }

    private void network_init() throws Exception{
        long send_time = 0;
        long server_time = 0;
        long current_time = 0;
        //sk = new Socket();


        while (true) {
            //连接服务器端


            try {

                sk = new Socket(server_address, server_port);
                is = sk.getInputStream();
                os = sk.getOutputStream();

            } catch (Exception e) {
                e.printStackTrace();
                Log.e("MYERROR", e.toString());
                network_delay.post(new Runnable() {
                    @Override
                    public void run() {
                        network_delay.setText("server unreachable");
                    }
                });
                sk.close();
                is.close();
                os.close();
                continue;
            }

            while (true) {

                if(Send_file_name != null){
                    send_file();
                    continue;
                }

                try {
                    os = sk.getOutputStream();
                    send_time = System.currentTimeMillis();
                    os.write((byte)0xF0);
                    os.flush();


                    map = read_data_from_socket();
                    server_time = Long.valueOf(new String(map.get("data"), 0, byteArrayToInt(map.get("len"))));

                    current_time = System.currentTimeMillis();

                    if(time_bias == 0) {          //只设置一次，动态变化会出bug
                        time_bias = (send_time + current_time - server_time * 2)/2;

                        Log.d("times", String.valueOf(send_time));
                        Log.d("times", String.valueOf(server_time));
                        Log.d("times", String.valueOf(current_time));
                        Log.d("times_bias", String.valueOf(time_bias));
                    }
                    final long delay = (current_time - send_time)/2;
                    network_delay.post(new Runnable() {
                        @Override
                        public void run() {
                            network_delay.setText(String.valueOf(delay) + "ms");
                        }
                    });


                    sleep(500);

                } catch (Exception e){
                    e.printStackTrace();
                    network_delay.post(new Runnable() {
                        @Override
                        public void run() {
                            network_delay.setText(String.valueOf(999999) + "ms");
                        }
                    });
                    sleep(1000);
                }
            }
        }
    }

    private void send_file(){
        long file_size = 0;
        int server_ret = 0;
        File file = new File(Send_file_name);
        if (file.exists() && file.isFile())
            file_size = file.length();
        else
            System.out.println("[Error] file cannot open");
        try {

            os = sk.getOutputStream();
            os.write((byte)0xF3);
            // 传输文件大小
            os.write(String.valueOf(file_size).getBytes());
            //Log.d("File_size", String.valueOf(file_size));
            os.flush();

            map = read_data_from_socket();
            server_ret = byteArratToInt_8(map.get("data"));

            if(server_ret == 0xE3){ // 开始传输文件
                os.write((byte)0xF1);
                os.flush();

                FileInputStream fin = new FileInputStream(file);
                byte[] sendByte = new byte[1024];
                // dout.writeUTF(file.getName());
                int length = 0;
                while((length = fin.read(sendByte, 0, sendByte.length))>0){
                    os.write(sendByte,0,length);
                    os.flush();
                }
                //sk.shutdownOutput();

                map = read_data_from_socket();
                server_ret = byteArratToInt_8(map.get("data"));

                if(server_ret == 0xE1)
                    Log.d("MYLOGDEBUG","Transfer Succ");
                Send_file_name = null;
            }


        }catch (Exception e){
            e.printStackTrace();
            Log.d("MYERROR", e.toString());
        }
    }


    private void bindViews() {

        btn_connect_server = (Button)findViewById(R.id.Connect_server);
        btn_connect_server.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //设置服务器ip
                server_address = inputIpConfig.getText().toString();
                new Thread() {
                    public void run() {
                        try {
                            network_init();
                        } catch (Exception e){
                            e.printStackTrace();
                        }
                    }
                }.start();
            }
        });

        btn_save = (ImageButton)findViewById(R.id.btn_save);
        btn_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (inputfilename.getText().toString().equals(""))
                {
                    //onStop();//传感器失效
                    Toast.makeText(MainActivity_2.this, "文件名不能为空，请输入", Toast.LENGTH_SHORT).show();
                    return;
                }

                btn_go.setEnabled(false);
                btn_save.setEnabled(false);

                CreateXls data_XLS=new CreateXls();//需要导入工程或者jxl包
                Calendar c = Calendar.getInstance();//可以对每个时间域单独修改

                int hour = c.get(Calendar.HOUR_OF_DAY);
                int minute = c.get(Calendar.MINUTE);
                int second = c.get(Calendar.SECOND);


                //File.separator是文件里面的斜杠的意思
                excelPath=data_XLS.getExcelDir()+ File.separator+inputfilename.getText().toString()+File.separator+
                        inputfilename.getText().toString()+"-"+String.valueOf(hour)+"-"+String.valueOf(minute)+"-"+String.valueOf(second)+".xls";

                excelLoc.setText(data_XLS.makedir(inputfilename.getText().toString()) + "-" + data_XLS.excelCreate(new File(excelPath)));
                f_path.setText(excelPath);

                WriteXls(AccList,GyrList,SeqList, TimeList);//核心代码：将采集的数据写入文件中
                SeqList.clear();
                TimeList.clear();
                AccList.clear();//清除链表数据
                GyrList.clear();
                times=0;
                //onStop();

                //传输文件
                Send_file_name = excelPath;

                btn_save.setEnabled(true);
                btn_go.setEnabled(true);

                Toast.makeText(MainActivity_2.this, "完成记录", Toast.LENGTH_SHORT).show();
                btn_save.setVisibility(View.INVISIBLE);
            }
        });
        btn_save.setVisibility(View.INVISIBLE);

        btn_go = (ImageButton)findViewById(R.id.btn_go);
        btn_go.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity_2.this,"按住以记录", Toast.LENGTH_SHORT );
            }
        });
        btn_go.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    btn_go.setImageResource(R.drawable.recording); // 更改图标

                    btn_connect_server.setVisibility(View.INVISIBLE);
                    inputIpConfig.setVisibility(View.INVISIBLE);

                    //Toast.makeText(MainActivity_2.this, "开始记录", Toast.LENGTH_SHORT).show();
                    times=0;
                    processState = true;

                    inputfilename.setEnabled(false);//数据一旦开始采集，不允许输入文件名
                    inputfilename.setVisibility(View.INVISIBLE);
                    btn_save.setEnabled(false);
                    btn_save.setVisibility(View.INVISIBLE);
                    excelLoc.setVisibility(View.INVISIBLE);
                    excelLoc.setText("");
                    f_path.setText("");

                    displaymode();

                    onStop();
                    onResume();

                    final RotateAnimation animation = new RotateAnimation(0.0f, 360.0f,
                            Animation.RELATIVE_TO_SELF, 0.5f,
                            Animation.RELATIVE_TO_SELF, 0.5f);
                    animation.setDuration( 1000 );


                    if(timer==null) {timer=new Timer();}
                    timer.schedule(new TimerTask()
                    {
                        int time_count = 0;
                        @Override
                        public void run() {
                            times++;
                            SeqList.add(times);
                            TimeList.add(System.currentTimeMillis()-time_bias);
                            AccList.add(AccData[0]);AccList.add(AccData[1]);AccList.add(AccData[2]);
                            GyrList.add(GyrData[0]);GyrList.add(GyrData[1]);GyrList.add(GyrData[2]);

                            if(time_count++ % 100 == 0)

                                btn_go.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        btn_go.startAnimation( animation );
                                    }
                                });
                        }
                    },0,timer_s);




                }
                if(event.getAction() == MotionEvent.ACTION_UP){
                    timer.cancel();
                    timer=null;
                    showtimes.setText("记录次数为"+ times);
                    processState = false;
                    onStop();

                    inputfilename.setVisibility(View.VISIBLE);
                    inputfilename.setEnabled(true);
                    btn_save.setVisibility(View.VISIBLE);
                    btn_save.setEnabled(true);
                    excelLoc.setVisibility(View.VISIBLE);



                    btn_go.setImageResource(R.drawable.begin);

                    Toast.makeText(MainActivity_2.this, "按下保存按钮以保存", Toast.LENGTH_SHORT).show();

                }

                return false;
            }
        });
    }



    @Override
    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this,
                sensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION),
                SensorManager.SENSOR_DELAY_FASTEST);
        sensorManager.registerListener(this,
                sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
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

    public static String getNowTime(){
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH-mm-ss,");
        Date date = new Date(System.currentTimeMillis());
        return simpleDateFormat.format(date);
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

                    AccData[0]=values[0];AccData[1]=values[1];AccData[2]=values[2];
                    showText1.setText(sb.toString());
                    Log.d("OUTPUT", getNowTime()+"["+values[0]+","+values[1]+","+values[2]+"]");

                    break;
                case Sensor.TYPE_ACCELEROMETER:
                    sb = new StringBuilder();
                    sb.append("陀螺仪传感器:\n");
                    sb.append("X:" + values[0] + "\n");
                    sb.append("Y:" + values[1] + "\n");
                    sb.append("Z:" + values[2] + "\n");

                    showText2.setText(sb.toString());

                    GyrData[0]=values[0]; GyrData[1]=values[1]; GyrData[2]=values[2];

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

    public void WriteXls(ArrayList<Float> accdata,ArrayList<Float> gyrdata,ArrayList<Integer> SeqList, ArrayList<Long> Timedata)
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

            for(int i=0,timerow=1;i<Timedata.size();i++)
            {
                if  (Timedata!=null && Timedata.get(i)!=null)
                {
                    Label label=new Label(1,timerow,String.valueOf(Timedata.get(i)));
                    sh.addCell(label);
                    timerow++;
                }
            }

            //逐个写入加速度数据到文件中去！
            for(int i=0,acc_Row=1;i<accdata.size();)
            {
                if  (accdata!=null && accdata.get(i)!=null)
                {
                    for(int j=2;j<5;j++)
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
                    for(int j=5;j<8;j++)
                    {
                        Label label=new Label(j,gyr_Row,String.valueOf(gyrdata.get(i)));
                        sh.addCell(label);
                        i++;
                    }
                    gyr_Row++;
                }
            }

            // flag位，记录按键是否按下
            for(int i=0,seqrow=1;i<SeqList.size();i++)
            {
                if  (SeqList!=null && SeqList.get(i)!=null)
                {
                    Label label=new Label(8,seqrow,String.valueOf(-1));
                    sh.addCell(label);
                    seqrow++;
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