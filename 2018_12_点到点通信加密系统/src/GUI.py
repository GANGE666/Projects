# coding=utf-8

import wx
import wx.xrc
import communication
import threading

# 消息队列
msg_queue = []


# GUI 界面
class MyFrame(wx.Frame):
    def __init__(self, parent):
        wx.Frame.__init__(self, parent, id=wx.ID_ANY, title=u"加密传输系统 by xxg", pos=wx.DefaultPosition,
                          size=wx.Size(440, 600), style=wx.CAPTION|wx.CLOSE_BOX|wx.MINIMIZE_BOX|wx.SYSTEM_MENU|wx.TAB_TRAVERSAL, name=u"Main")
        self.SetSizeHints(wx.DefaultSize, wx.DefaultSize)
        self.SetForegroundColour(wx.SystemSettings.GetColour(wx.SYS_COLOUR_BTNTEXT))
        self.SetBackgroundColour(wx.SystemSettings.GetColour(wx.SYS_COLOUR_WINDOW))

        bSizer1 = wx.BoxSizer(wx.VERTICAL)

# 监听 & 连接按钮
        fgSizer0 = wx.FlexGridSizer(0, 2, 0, 0)
        fgSizer0.SetFlexibleDirection(wx.BOTH)
        fgSizer0.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)
        self.m_button_listen = wx.Button(self, wx.ID_ANY, u"监听", wx.DefaultPosition, wx.DefaultSize, 0)
        fgSizer0.Add(self.m_button_listen, 0, wx.ALL, 5)
        self.m_button_connect = wx.Button(self, wx.ID_ANY, u"连接", wx.DefaultPosition, wx.DefaultSize, 0)
        fgSizer0.Add(self.m_button_connect, 0, wx.ALL, 5)

        bSizer1.Add(fgSizer0, 0, wx.EXPAND, 5)
# 消息框
        fgSizer5 = wx.FlexGridSizer(0, 1, 0, 0)
        fgSizer5.SetFlexibleDirection(wx.BOTH)
        fgSizer5.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)

        self.area_text = wx.TextCtrl(self, -1, u'连接尚未建立，按以下键继续操作\n监听: 等待连接\n连接: 连接目标主机', size=(400, 380),
                                     style=(wx.TE_MULTILINE | wx.TE_DONTWRAP))
        self.area_text.SetInsertionPoint(0)

        fgSizer5.Add(self.area_text, 0, wx.ALL, 5)
        bSizer1.Add(fgSizer5, 0, wx.EXPAND, 5)

# File Select
        fgSizer2 = wx.FlexGridSizer(0, 2, 0, 0)
        fgSizer2.SetFlexibleDirection(wx.BOTH)
        fgSizer2.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)

        self.m_staticText3 = wx.StaticText(self, wx.ID_ANY, u"发送文件", wx.DefaultPosition, wx.DefaultSize, 0)
        self.m_staticText3.Wrap(-1)
        self.m_staticText3.SetFont(wx.Font(wx.NORMAL_FONT.GetPointSize(), 70, 90, 90, False, wx.EmptyString))

        fgSizer2.Add(self.m_staticText3, 0, wx.ALL, 5)

        self.m_filePicker1 = wx.FilePickerCtrl(self, wx.ID_ANY, wx.EmptyString, u"Select a file", u"*.*",
                                               wx.DefaultPosition, wx.Size(300, -1),
                                               wx.FLP_DEFAULT_STYLE | wx.FLP_SMALL)
        fgSizer2.Add(self.m_filePicker1, 0, wx.ALL, 5)

        bSizer1.Add(fgSizer2, 0, wx.EXPAND, 5)

# 文本输入框及发送按钮
        fgSizer4 = wx.FlexGridSizer(0, 2, 0, 0)
        fgSizer4.SetFlexibleDirection(wx.BOTH)
        fgSizer4.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)

        self.senf_text = wx.TextCtrl(self, wx.ID_ANY, wx.EmptyString, wx.Point(-1, -1), wx.Size(300, 80), style=(wx.TE_MULTILINE | wx.TE_DONTWRAP))
        self.input_str = self.senf_text.GetValue()

        fgSizer4.Add(self.senf_text, 0, wx.ALL, 5)

        self.m_button_send = wx.Button(self, wx.ID_ANY, u"发送", wx.DefaultPosition, wx.DefaultSize, 0)
        fgSizer4.Add(self.m_button_send, 0, wx.ALL, 5)

        bSizer1.Add(fgSizer4, 0, wx.EXPAND, 5)
# End

        self.SetSizer(bSizer1)
        self.Layout()

        self.Centre(wx.BOTH)

        self.m_button_listen.Bind(wx.EVT_BUTTON, self.m_button_listen_OnButtonClick)
        self.m_button_connect.Bind(wx.EVT_BUTTON, self.m_button_connect_OnButtonClick)
        self.m_button_send.Bind(wx.EVT_BUTTON, self.m_button_send_OnButtonClick)

    def __del__(self):
        pass

    # 监听按钮事件，开启监听线程
    def m_button_listen_OnButtonClick(self, event):
        listen_thread = threading.Thread(target=self.listen_thread)
        listen_thread.start()

    # 连接按钮事件，开启连接线程
    def m_button_connect_OnButtonClick(self, event):
        connect_thread = threading.Thread(target=self.connect_thread)
        connect_thread.start()
        pass

    # 发送按钮事件，发送文字或文件
    def m_button_send_OnButtonClick(self, event):
        try:
            text = self.senf_text.GetValue()
            if text != '':
                msg_queue.append("Sending text: %s\n" % text)
                self.connect.send_string(text)
                self.senf_text.Clear()

            f_path = self.m_filePicker1.GetPath()
            if f_path != '':
                msg_queue.append("Sending file: %s\n" % f_path)
                self.connect.send_file(f_path)
                self.m_filePicker1.Refresh()
        except Exception as e:
            print("Send Fail")
            print(e)
            msg_queue.append("\n发送失败，连接尚未建立或已经断开，请重新建立连接\n")

    # 监听线程
    def listen_thread(self):
        self.listen = communication.Communication(mode='LISTEN', msg_queue=msg_queue)
        self.listen.listen_manager()

    # 连接线程
    def connect_thread(self):
        self.connect = communication.Communication(mode='CONNECT', msg_queue=msg_queue)

    # 显示结果线程
    def disp_text(self):
        self.area_text.Clear()
        msg_queue.append("连接尚未建立，按以下键继续操作\n监听: 等待连接\n连接: 连接目标主机\n")

        while True:
            if len(msg_queue) != 0:
                self.area_text.AppendText(msg_queue.pop(0))

    # 开启显示结果线程
    def disp_text_begin(self):
        disp_text = threading.Thread(target=self.disp_text)
        disp_text.start()

if __name__ == "__main__":
    app = wx.App(False)
    frame = MyFrame(None)
    frame.Show(True)
    frame.disp_text_begin()
    app.MainLoop()
