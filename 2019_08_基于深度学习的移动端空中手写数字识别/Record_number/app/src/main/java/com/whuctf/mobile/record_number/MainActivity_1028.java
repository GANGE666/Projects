package com.whuctf.mobile.record_number;

import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.RandomAccessFile;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;


public class MainActivity_1028 extends AppCompatActivity implements View.OnClickListener,SensorEventListener {
    /*
     * 变量定义-----------------------------------------------------------------------------
     * */

    // UI控件
    private SensorManager sensorManager;
    private TextView modeText;
    private TextView showText1;
    private TextView showText2;
    private TextView showtimes;
    private TextView f_path;
    private TextView textViewScore;

    private ImageView trans_image_view;

    private ImageButton btn_go;
    private ImageButton btn_save;

    private boolean processState = false;   //标记当前是否已经在记录
    private int times=0;

    // 记录传感器数据用的变量
    Timer timer=null;
    ArrayList<Integer> SeqList=new ArrayList<Integer>();//序号
    ArrayList<Long> TimeList=new ArrayList<Long>();//时间戳
    ArrayList<String> TimeList2 = new ArrayList<String>(); // 时间
    ArrayList <Float> AccList=new ArrayList<Float>();//加速度
    ArrayList <Float> GyrList=new ArrayList<Float>();//陀螺仪
    private float AccData[]=new float[3];//陀螺仪
    private float GyrData[]=new float[3];//加速度

    // 几种采样频率
    private int mode=2;
    public static final int SENSOR_RATE_NORMAL=20;
    public static final int SENSOR_RATE_UI=10;
    public static final int SENSOR_RATE_GAME=5;
    public static final int SENSOR_RATE_FASTEST=0;
    private int hzmode[]= {SENSOR_RATE_FASTEST,SENSOR_RATE_GAME,SENSOR_RATE_UI,SENSOR_RATE_NORMAL};

    private int timer_s = hzmode[mode]; //timer采样频率

    // 摇一摇动画
    private ObjectAnimator anim;
    private boolean isShake = false;
    private int isShakeCounter = 0;

    private int ShakeCounter = 0;

    /*
     * 变量定义-----------------------------------------------------------------------------
     * */


    // OnCreate 绑定控件
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_2);
        modeText = (TextView) findViewById(R.id.modeText);
        showText1 = (TextView) findViewById(R.id.showText1);
        showText2 = (TextView) findViewById(R.id.showText2);
        showtimes = (TextView) findViewById(R.id.showtimes);
        f_path = (TextView)findViewById(R.id.path);
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        textViewScore = (TextView) findViewById(R.id.textView_score);


        trans_image_view = (ImageView) findViewById(R.id.trans_image);

        int random = new Random().nextInt(859) + 1;       // 969466836

        StringBuilder v4;
        String v3 = "";
        v4 = new StringBuilder();
        v4.append(random);
        v4.append(v3);

        Log.d("Randdddom", v4.toString());

        // 初始化摇一摇动画的类
        anim = ObjectAnimator.ofFloat(trans_image_view,"rotation",0f,45f,-30f,0f);
        anim.setDuration(500);
        anim.setRepeatCount(ValueAnimator.INFINITE);

        bindViews();
        displaymode();

    }


    /*
     * 绑定按钮事件-----------------------------------------------------------------------------
     * */

    private void bindViews() {


        btn_save = (ImageButton)findViewById(R.id.btn_save);
        // 保存按钮 绑定点击事件
        btn_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 记录传感器数据的函数
                // 传感器数据在SeqList TimeList2 AccList GyrList中
                // 调用writeData即可将数据写入文件

                // 使按钮不可点击
                btn_go.setEnabled(false);
                btn_save.setEnabled(false);

                writeData();

                // 清空列表
                SeqList.clear();
                TimeList2.clear();
                AccList.clear();//清除链表数据
                GyrList.clear();
                times=0;
                //onStop();

                Toast.makeText(MainActivity_1028.this, "完成记录", Toast.LENGTH_SHORT).show();
                // 设置为不可见
                btn_save.setVisibility(View.INVISIBLE);
            }
        });
        btn_save.setVisibility(View.INVISIBLE);



        btn_go = (ImageButton)findViewById(R.id.btn_go);
        // 记录按钮 绑定点击事件
        btn_go.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 按下按钮
                if(processState == false){
                    ShakeCounter = 0;
                    textViewScore.setText(Integer.toString(ShakeCounter));

                    btn_go.setImageResource(R.drawable.recording); // 更改图标

                    //Toast.makeText(MainActivity_1028.this, "开始记录", Toast.LENGTH_SHORT).show();
                    times=0;
                    processState = true;

                    btn_save.setEnabled(false);
                    btn_save.setVisibility(View.INVISIBLE);
                    f_path.setText("");

                    // 显示采样频率
                    displaymode();

                    onStop();
                    onResume();

                    // 使按钮旋转
                    final RotateAnimation animation = new RotateAnimation(0.0f, 360.0f,
                            Animation.RELATIVE_TO_SELF, 0.5f,
                            Animation.RELATIVE_TO_SELF, 0.5f);
                    animation.setDuration( 1000 );


                    // 计时器
                    if(timer==null) {timer=new Timer();}
                    timer.schedule(new TimerTask()
                    {
                        int time_count = 0;
                        @Override
                        public void run() {
                            // 记录传感器数据及时间
                            times++;
                            SeqList.add(times);
                            TimeList2.add(getNowTime());
                            AccList.add(AccData[0]);AccList.add(AccData[1]);AccList.add(AccData[2]);
                            GyrList.add(GyrData[0]);GyrList.add(GyrData[1]);GyrList.add(GyrData[2]);

                            if(time_count++ % 100 == 0) {

                                // 按钮旋转
                                btn_go.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        btn_go.startAnimation(animation);
                                    }
                                });
                            }

                            // 修改isShakeCounter的值，使其2s后才能再摇
                            if(isShake == true){
                                isShakeCounter -= 1;                    // 计数器
                                if(isShakeCounter <= 0){
                                    isShake = false;

                                    trans_image_view.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            anim.cancel();              // 结束动画
                                        }
                                    });

                                }

                            }
                        }
                    },0,timer_s);




                }
                // 松开按钮
                else if(processState == true){
                    timer.cancel();
                    timer=null;
                    showtimes.setText("记录次数为"+ times);
                    processState = false;
                    onStop();

                    btn_save.setVisibility(View.VISIBLE);
                    btn_save.setEnabled(true);

                    // 更改图片
                    btn_go.setImageResource(R.drawable.begin);

                    Toast.makeText(MainActivity_1028.this, "按下保存按钮以保存", Toast.LENGTH_SHORT).show();

                    // 重置摇一摇状态，结束摇一摇动画
                    isShake = false;
                    trans_image_view.post(new Runnable() {
                        @Override
                        public void run() {
                            anim.cancel();
                        }
                    });
                }

//                return false;
            }
        });
    }

    /*
     * 绑定按钮事件-----------------------------------------------------------------------------
     * */

    /*
     * 监听传感器数据-----------------------------------------------------------------------------
     * */

    // Resume时开始采集 传感器数据
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

    // Stop时，停止收集传感器数据
    @Override
    protected void onStop() {
        super.onStop();
        sensorManager.unregisterListener(this);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    // 传感器数据发生变化时
    @Override
    public void onSensorChanged(SensorEvent event) {
        int type = event.sensor.getType();
        StringBuilder sb;
        float[] values = event.values;
        if(processState) {
            switch(type) {
                // 线性加速度传感器
                case Sensor.TYPE_LINEAR_ACCELERATION:

                    sb = new StringBuilder();
                    sb.append("线性加速度传感器:\n");
                    sb.append("X:" + values[0] + "\n");
                    sb.append("Y:" + values[1] + "\n");
                    sb.append("Z:" + values[2] + "\n");

                    // 将数据保存到 value
                    AccData[0]=values[0];AccData[1]=values[1];AccData[2]=values[2];
                    showText1.setText(sb.toString());
                    // Log.d("OUTPUT", getNowTime()+"["+values[0]+","+values[1]+","+values[2]+"]");


                    // 检测是否超过摇一摇阈值 阈值为10
                    if(Math.abs(values[0]) > 15 || Math.abs(values[1]) > 15 || Math.abs(values[2]) > 15){
                        ShakeCounter += 1;
                        textViewScore.setText(Integer.toString(ShakeCounter));
                        // 如果摇一摇的动画还在转，返回
                        if (isShake) {
                            return;
                        }
                        isShake = true;
                        isShakeCounter = 200;           // 用来计时
                        Log.d("SHAKE", "SHAKED!!");
                        Toast.makeText(MainActivity_1028.this, "Shake!!", Toast.LENGTH_SHORT).show();
                        anim.start();                   // 开始动画
                    }

                    break;
                // 陀螺仪
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

    /*
     * 监听传感器数据-----------------------------------------------------------------------------
     * */


    /*
    * 写文件--------------------------------------------------------------------------------------
    * */

    // 写数据
    // 调用这个函数就可以把线性加速度的数据写进txt中了
    private void writeData() {
        String filecontent = new String();
        for(int i=0; i<TimeList2.size(); i++){
            filecontent += getNowTime()+",[";
            for(int j=0; j<3; j++){
                filecontent += AccList.get(3*i+j)+",";
            }
            filecontent += "]\n";
        }
        String filePath = "/sdcard/Record_num/";
        String fileName = getNowTime()+".txt";
        f_path.setText(filePath+fileName);
        writeTxtToFile(filecontent, filePath, fileName);
    }

    // 将字符串写入到文本文件中
    private void writeTxtToFile(String strcontent, String filePath, String fileName) {
        //生成文件夹之后，再生成文件，不然会出错
        makeFilePath(filePath, fileName);

        String strFilePath = filePath + fileName;
        // 每次写入时，都换行写
        String strContent = strcontent + "\r\n";
        try {
            File file = new File(strFilePath);
            if (!file.exists()) {
                Log.d("TestFile", "Create the file:" + strFilePath);
                file.getParentFile().mkdirs();
                file.createNewFile();
            }
            RandomAccessFile raf = new RandomAccessFile(file, "rwd");
            raf.seek(file.length());
            raf.write(strContent.getBytes());
            raf.close();
        } catch (Exception e) {
            Log.e("TestFile", "Error on write File:" + e);
        }
    }

    //生成文件
    private File makeFilePath(String filePath, String fileName) {
        File file = null;
        makeRootDirectory(filePath);
        try {
            file = new File(filePath + fileName);
            if (!file.exists()) {
                file.createNewFile();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return file;
    }

    //生成文件夹
    private static void makeRootDirectory(String filePath) {
        File file = null;
        try {
            file = new File(filePath);
            if (!file.exists()) {
                file.mkdir();
            }
        } catch (Exception e) {
            Log.i("error:", e + "");
        }
    }

    /*
     * 写文件--------------------------------------------------------------------------------------
     * */

    /*
     * 杂七杂八的函数-------------------------------------------------------------------------------
     * */

    // 获取当前时间字符串
    public static String getNowTime(){
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH-mm-ss");
        Date date = new Date(System.currentTimeMillis());
        return simpleDateFormat.format(date);
    }

    // 显示采样频率
    public void displaymode() {
        switch (hzmode[mode]) {
            case SENSOR_RATE_FASTEST:
                modeText.setText("采样频率 "+ String.valueOf(SENSOR_RATE_FASTEST) + "ms");
                break;
            case SENSOR_RATE_GAME:
                modeText.setText("采样频率 "+ String.valueOf(SENSOR_RATE_GAME) + "ms");
                break;
            case SENSOR_RATE_NORMAL:
                modeText.setText("采样频率 "+ String.valueOf(SENSOR_RATE_NORMAL) + "ms");
                break;
            case SENSOR_RATE_UI:
                modeText.setText("采样频率 "+ String.valueOf(SENSOR_RATE_UI) + "ms");
                break;
            default:
                break;
        }
    }

    @Override
    public void onClick(View arg0) {
        // TODO 自动生成的方法存根
    }

    /*
     * 杂七杂八的函数-------------------------------------------------------------------------------
     * */

}


