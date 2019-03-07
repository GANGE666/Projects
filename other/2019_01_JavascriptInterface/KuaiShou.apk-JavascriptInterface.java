/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->accessWalletService(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e298
*/

    public void accessWalletService(String p2, String p3)
    {
        this.a.c(p2, p3);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->addseniorWz(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e2b4
*/

    public void addseniorWz(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->audioSeekTo(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e2d8
*/

    public void audioSeekTo(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->audioSpeedFF(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e2fc
*/

    public void audioSpeedFF(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->bdLogin(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e320
*/

    public void bdLogin(String p4, String p5, String p6)
    {
        com.baidu.wallet.core.utils.LogUtil.d("BLightApp", new StringBuilder("bdLogin. param1 = ").append(p4).append(", param2 = ").append(p5).append(", param3 = ").append(p6).toString());
        if (this.a != null) {
            this.a.b(p4, p5, p6);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->callShare(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e39c
*/

    public void callShare(String p4, String p5, String p6)
    {
        com.baidu.wallet.core.utils.LogUtil.d("BLightApp", new StringBuilder("callShare. param1 = ").append(p4).append(", param2 = ").append(p5).append(", param3 = ").append(p6).toString());
        if (this.a != null) {
            this.a.c(p4, p5, p6);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->closeWindow()V [access_flags=public] @ 0x32e418
*/

    public void closeWindow()
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        this.a.a();
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->cloudaLaunchCamera(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e448
*/

    public void cloudaLaunchCamera(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->cloudaLaunchGallery(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e46c
*/

    public void cloudaLaunchGallery(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->dopay(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String; Z)V [access_flags=public] @ 0x32e490
*/

    public void dopay(String p12, String p13, String p14, boolean p15)
    {
        String v4 = 0;
        if (!android.text.TextUtils.isEmpty(p14)) {
            String v0_0 = "";
            String[] v6 = p14.split("&");
            if ((v6 != null) && (v6.length > 0)) {
                int v7 = v6.length;
                int v5 = 0;
                String v1_1 = 0;
                String v3_0 = 1;
                while (v5 < v7) {
                    int v8_0 = v6[v5];
                    if ((v1_1 == null) && ((v8_0.startsWith("order_no=")) && (v8_0.length() > 9))) {
                        v0_0 = v8_0.substring(9);
                        v1_1 = 1;
                    }
                    if ((v8_0.startsWith("goods_category=")) && (v8_0.length() > 15)) {
                        if (v1_1 != null) {
                            if ((v4 != null) && (!android.text.TextUtils.isEmpty(v0_0))) {
                                try {
                                    p14 = new StringBuilder().append(p14).append("&app_coupon=").append(java.net.URLEncoder.encode(com.baidu.android.pay.SafePay.getInstance().encrypt(new StringBuilder().append(v0_0).append(";type:1;app_store_").append(com.baidu.paysdk.ui.LightappBrowseActivity.EXT_STORE_NO).toString()), "UTF-8")).toString();
                                } catch (String v0_9) {
                                    v0_9.printStackTrace();
                                }
                            }
                            if ((!p14.contains("ext_store_no")) && (!android.text.TextUtils.isEmpty(com.baidu.paysdk.ui.LightappBrowseActivity.EXT_STORE_NO))) {
                                p14 = new StringBuilder().append(p14).append("&ext_store_no=").append(com.baidu.paysdk.ui.LightappBrowseActivity.EXT_STORE_NO).toString();
                            }
                            this.a.a(p12, p13, p14, p15);
                            return;
                        } else {
                            v3_0 = 0;
                        }
                    }
                    v5++;
                }
                v4 = v3_0;
            }
        }
        this.a.a(p12, p13, p14, p15);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->followSite(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e64c
*/

    public void followSite(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getBattery(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e670
*/

    public void getBattery(String p2, String p3)
    {
        this.a.a(p2, p3);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getCurrentPosition(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e68c
*/

    public void getCurrentPosition(String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.d("BLightApp", new StringBuilder("getCurrentPosition. param1 = ").append(p4).append(", param2 = ").append(p5).toString());
        if (this.a != null) {
            this.a.b(p4, p5);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getDeviceInfo(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e6f4
*/

    public void getDeviceInfo(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getGlobalizationInfo()Ljava/lang/String; [access_flags=public] @ 0x32e718
*/

    public String getGlobalizationInfo()
    {
        return "";
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getNetworkType(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e730
*/

    public void getNetworkType(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getPushToken(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e754
*/

    public void getPushToken(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->getWebKitPluginInfo(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e778
*/

    public void getWebKitPluginInfo(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->initpay(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e79c
*/

    public void initpay(String p2, String p3, String p4)
    {
        this.a.a(p2, p3, p4);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->invokeThirdApp(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e7b8
*/

    public void invokeThirdApp(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->launchSeniorVoiceRecognition(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e7dc
*/

    public void launchSeniorVoiceRecognition(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->playAudio(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e800
*/

    public void playAudio(String p3, String p4, String p5, String p6)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->postFile(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e824
*/

    public void postFile(String p3, String p4, String p5, String p6)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->queryWzStatus(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e848
*/

    public void queryWzStatus(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->setVolume(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e86c
*/

    public void setVolume(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->shareB64Img(Ljava/lang/String;)V [access_flags=public] @ 0x32e890
*/

    public void shareB64Img(String p3)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->startListenBattery(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e8b4
*/

    public void startListenBattery(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->startListenKeyboard(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e8d8
*/

    public void startListenKeyboard(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->startListenLocation(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e8fc
*/

    public void startListenLocation(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->startQRcode(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e920
*/

    public void startQRcode(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->startRecording(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e944
*/

    public void startRecording(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->stopListenBattery(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e968
*/

    public void stopListenBattery(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->stopListenKeyboard(Ljava/lang/String; Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e98c
*/

    public void stopListenKeyboard(String p3, String p4, String p5)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->stopListenLocation(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e9b0
*/

    public void stopListenLocation(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/lightapp/LightappJsClient;->stopRecording(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x32e9d4
*/

    public void stopRecording(String p3, String p4)
    {
        com.baidu.wallet.core.utils.LogUtil.errord("BLightApp", "This method is not implemented!");
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/paysdk/ui/PassNormalizeActivity$JavascriptInterfaceImpl;->authorized_response(Ljava/lang/String;)V [access_flags=public] @ 0x338584
*/

    public void authorized_response(String p5)
    {
        String v2_0 = com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0, p5);
        if (v2_0 == null) {
            com.baidu.wallet.core.utils.GlobalUtils.toast(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0), "\u5e10\u53f7\u8865\u5168\u5931\u8d25");
        } else {
            if ((android.text.TextUtils.isEmpty(((CharSequence) v2_0.get("pass_error_code")))) || (!"0".equals(v2_0.get("pass_error_code")))) {
                String v0_7 = 0;
            } else {
                v0_7 = 1;
            }
            if (v0_7 == null) {
                if (android.text.TextUtils.isEmpty(((CharSequence) v2_0.get("pass_error_msg")))) {
                    com.baidu.wallet.core.utils.GlobalUtils.toast(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0), "\u5e10\u53f7\u8865\u5168\u5931\u8d25");
                } else {
                    com.baidu.wallet.core.utils.GlobalUtils.toast(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0), ((CharSequence) v2_0.get("pass_error_msg")));
                }
                String v0_21 = ((String) v2_0.get("pass_error_code"));
                if (android.text.TextUtils.isEmpty(v0_21)) {
                    v0_21 = "-12345";
                }
                com.baidu.wallet.base.stastics.PayStatisticsUtil.onEvent(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0), "normalizeComplete", v0_21);
            } else {
                com.baidu.wallet.core.utils.PassUtil.backNormalized(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0).getApplicationContext(), this.this$0.c, v2_0);
                com.baidu.wallet.base.stastics.PayStatisticsUtil.onEvent(com.baidu.paysdk.ui.PassNormalizeActivity.a(this.this$0), "normalizeComplete", "success");
                this.this$0.finish();
            }
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_fast_reg()V [access_flags=public final] @ 0x362b04
*/

    public final void action_fast_reg()
    {
        if (com.baidu.sapi2.SapiWebView.l(this.a) == null) {
            this.a.loadFastReg();
        } else {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$10(this));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_forget_pwd()Ljava/lang/String; [access_flags=public final] @ 0x362b48
*/

    public final String action_forget_pwd()
    {
        String v0_2 = new boolean[1];
        v0_2[0] = 0;
        boolean[] v1_1 = new boolean[1];
        v1_1[0] = 0;
        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$11(this, v0_2, v1_1));
        while (!v1_1[0]) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException v2_2) {
                com.baidu.sapi2.utils.L.e(v2_2);
            }
        }
        String v0_1;
        if (v0_2[0] == null) {
            v0_1 = "0";
        } else {
            v0_1 = "1";
        }
        return v0_1;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_lecai_login()V [access_flags=public final] @ 0x362bc8
*/

    public final void action_lecai_login()
    {
        if (com.baidu.sapi2.SapiWebView.v(this.a) != null) {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$13(this));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_nuomi_login()V [access_flags=public final] @ 0x362c00
*/

    public final void action_nuomi_login()
    {
        if (com.baidu.sapi2.SapiWebView.u(this.a) != null) {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$12(this));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_received_sms_code(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public final] @ 0x362c38
*/

    public final void action_received_sms_code(String p5, String p6)
    {
        com.baidu.sapi2.SapiWebView.a(this.a, com.baidu.sapi2.SapiWebView.r(this.a));
        com.baidu.sapi2.SapiWebView.r(this.a).a = p5;
        com.baidu.sapi2.SapiWebView.r(this.a).b = p6;
        com.baidu.sapi2.SapiWebView.r(this.a).postDelayed(com.baidu.sapi2.SapiWebView.k(this.a), 15000);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_remove_share_account(Ljava/lang/String;)V [access_flags=public final] @ 0x362ca4
*/

    public final void action_remove_share_account(String p4)
    {
        if (!android.text.TextUtils.isEmpty(p4)) {
            com.baidu.sapi2.share.b v1_1 = com.baidu.sapi2.SapiAccountManager.getInstance().getShareAccounts().iterator();
            while (v1_1.hasNext()) {
                com.baidu.sapi2.SapiAccount v0_3 = ((com.baidu.sapi2.SapiAccount) v1_1.next());
                if (p4.equals(v0_3.uid)) {
                    com.baidu.sapi2.share.b.a().b(v0_3);
                    break;
                }
            }
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_share_accounts_view_btn_clicked()V [access_flags=public final] @ 0x362d10
*/

    public final void action_share_accounts_view_btn_clicked()
    {
        if (com.baidu.sapi2.SapiWebView.m(this.a) != null) {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$1(this));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_qzone_webview()V [access_flags=public final] @ 0x362d48
*/

    public final void action_social_qzone_webview()
    {
        if (com.baidu.sapi2.SapiWebView.t(this.a) != null) {
            android.os.Message v0_3 = new android.os.Message();
            v0_3.what = com.baidu.sapi2.utils.enums.SocialType.QZONE.getType();
            com.baidu.sapi2.SapiWebView.t(this.a).sendMessage(v0_3);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_renren_webview()V [access_flags=public final] @ 0x362d98
*/

    public final void action_social_renren_webview()
    {
        if (com.baidu.sapi2.SapiWebView.t(this.a) != null) {
            android.os.Message v0_3 = new android.os.Message();
            v0_3.what = com.baidu.sapi2.utils.enums.SocialType.RENREN.getType();
            com.baidu.sapi2.SapiWebView.t(this.a).sendMessage(v0_3);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_sina_sso()V [access_flags=public final] @ 0x362de8
*/

    public final void action_social_sina_sso()
    {
        this.a.loadSinaSSOLogin();
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_sina_weibo_webview()V [access_flags=public final] @ 0x362e04
*/

    public final void action_social_sina_weibo_webview()
    {
        if (com.baidu.sapi2.SapiWebView.t(this.a) != null) {
            android.os.Message v0_3 = new android.os.Message();
            v0_3.what = com.baidu.sapi2.utils.enums.SocialType.SINA_WEIBO.getType();
            com.baidu.sapi2.SapiWebView.t(this.a).sendMessage(v0_3);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_tx_weibo_webview()V [access_flags=public final] @ 0x362e54
*/

    public final void action_social_tx_weibo_webview()
    {
        if (com.baidu.sapi2.SapiWebView.t(this.a) != null) {
            android.os.Message v0_3 = new android.os.Message();
            v0_3.what = com.baidu.sapi2.utils.enums.SocialType.TENCENT_WEIBO.getType();
            com.baidu.sapi2.SapiWebView.t(this.a).sendMessage(v0_3);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_social_weixin_sso()V [access_flags=public final] @ 0x362ea4
*/

    public final void action_social_weixin_sso()
    {
        if (com.baidu.sapi2.SapiWebView.t(this.a) != null) {
            android.os.Message v0_3 = new android.os.Message();
            v0_3.what = com.baidu.sapi2.utils.enums.SocialType.WEIXIN.getType();
            com.baidu.sapi2.SapiWebView.t(this.a).sendMessage(v0_3);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_unite_verify(Ljava/lang/String;)V [access_flags=public final] @ 0x362ef4
*/

    public final void action_unite_verify(String p3)
    {
        if (com.baidu.sapi2.SapiWebView.x(this.a) != null) {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$15(this, p3));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->action_voice_login()V [access_flags=public final] @ 0x362f2c
*/

    public final void action_voice_login()
    {
        if (com.baidu.sapi2.SapiWebView.w(this.a) != null) {
            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$14(this));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->authorized_response(Ljava/lang/String;)V [access_flags=public final] @ 0x362f64
*/

    public final void authorized_response(String p2)
    {
        this.authorized_response(p2, 0);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->authorized_response(Ljava/lang/String; I)V [access_flags=public final] @ 0x362f80
*/

    public final void authorized_response(String p5, int p6)
    {
        if (p6 == 1) {
            com.baidu.sapi2.SapiWebView v0_14 = com.baidu.sapi2.SapiWebView.a(com.baidu.sapi2.SapiWebView.d(this.a).context, p5);
            if (v0_14 != null) {
                if ((v0_14.errorCode != 0) && (v0_14.errorCode != 110000)) {
                    if (com.baidu.sapi2.SapiWebView.n(this.a) != null) {
                        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$6(this, v0_14));
                    }
                } else {
                    if (com.baidu.sapi2.SapiWebView.n(this.a) != null) {
                        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$5(this));
                    }
                }
            } else {
                if (com.baidu.sapi2.SapiWebView.n(this.a) != null) {
                    this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$4(this));
                }
            }
        }
        if (p6 == 0) {
            com.baidu.sapi2.SapiWebView v0_8 = com.baidu.sapi2.SapiWebView.a(p5);
            if (v0_8 != null) {
                if ((com.baidu.sapi2.SapiWebView.o(this.a) == null) || (!v0_8.newReg)) {
                    if ((v0_8.errorCode != 0) && (v0_8.errorCode != 110000)) {
                        if (com.baidu.sapi2.SapiWebView.n(this.a) != null) {
                            this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$8(this, v0_8));
                        }
                    } else {
                        com.baidu.sapi2.SapiWebView.c(this.a, v0_8);
                    }
                } else {
                    com.baidu.sapi2.SapiWebView.a(this.a, v0_8);
                    com.baidu.sapi2.SapiWebView.b(this.a, v0_8);
                }
            } else {
                if (com.baidu.sapi2.SapiWebView.n(this.a) != null) {
                    this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$7(this));
                }
            }
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->back()V [access_flags=public final] @ 0x3630e0
*/

    public final void back()
    {
        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$3(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->config_canshare_accounts()Ljava/lang/String; [access_flags=public final] @ 0x363108
*/

    public final String config_canshare_accounts()
    {
        String v0_3;
        String v0_2 = com.baidu.sapi2.SapiAccount.toJSONArray(com.baidu.sapi2.SapiAccountManager.getInstance().getShareAccounts());
        if (v0_2 != null) {
            v0_3 = v0_2.toString();
        } else {
            v0_3 = 0;
        }
        return v0_3;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->config_fastlogin_features()Ljava/lang/String; [access_flags=public final] @ 0x363144
*/

    public final String config_fastlogin_features()
    {
        int v0_12;
        java.util.List v2 = com.baidu.sapi2.SapiWebView.d(this.a).fastLoginFeatureList;
        if (v2 != null) {
            if ((!com.baidu.sapi2.d.a(this.a.getContext()).b()) && (v2.contains(com.baidu.sapi2.utils.enums.FastLoginFeature.DEVICE_LOGIN))) {
                v2.remove(com.baidu.sapi2.utils.enums.FastLoginFeature.DEVICE_LOGIN);
            }
            StringBuilder v3_1 = new StringBuilder();
            if (!com.baidu.sapi2.d.a(this.a.getContext()).c()) {
                int v1 = 0;
                while (v1 < v2.size()) {
                    int v0_14 = ((com.baidu.sapi2.utils.enums.FastLoginFeature) v2.get(v1));
                    if (v1 != 0) {
                        v3_1.append(",").append(v0_14.getStrValue());
                    } else {
                        v3_1.append(v0_14.getStrValue());
                    }
                    v1++;
                }
                v0_12 = v3_1.toString();
            } else {
                v0_12 = v3_1.toString();
            }
        } else {
            v0_12 = 0;
        }
        return v0_12;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->config_login_share_strategy()Ljava/lang/String; [access_flags=public final] @ 0x363238
*/

    public final String config_login_share_strategy()
    {
        return com.baidu.sapi2.SapiWebView.d(this.a).loginShareStrategy().getStrValue();
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->finish()V [access_flags=public final] @ 0x363268
*/

    public final void finish()
    {
        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$2(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->get_preset_phone_number()Ljava/lang/String; [access_flags=public final] @ 0x363290
*/

    public final String get_preset_phone_number()
    {
        String v0_2;
        if (!com.baidu.sapi2.utils.SapiUtils.isValidPhoneNumber(com.baidu.sapi2.SapiWebView.d(this.a).presetPhoneNumber)) {
            v0_2 = "";
        } else {
            v0_2 = com.baidu.sapi2.SapiWebView.d(this.a).presetPhoneNumber;
        }
        return v0_2;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->loginWithDeviceId()V [access_flags=public final] @ 0x3632d8
*/

    public final void loginWithDeviceId()
    {
        if (com.baidu.sapi2.SapiWebView.z(this.a) != null) {
            com.baidu.sapi2.SapiWebView.z(this.a).sendMessage(new android.os.Message());
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->loginWithQRCode()V [access_flags=public final] @ 0x363318
*/

    public final void loginWithQRCode()
    {
        if (com.baidu.sapi2.SapiWebView.y(this.a) != null) {
            com.baidu.sapi2.SapiWebView.y(this.a).sendMessage(new android.os.Message());
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->sapi_cloud_log_op(Ljava/lang/String; Ljava/lang/String; J J)V [access_flags=public final] @ 0x363358
*/

    public final void sapi_cloud_log_op(String p1, String p2, long p3, long p5)
    {
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->sapi_cloud_log_op_net(Ljava/lang/String; Ljava/lang/String; J J)V [access_flags=public final] @ 0x36336c
*/

    public final void sapi_cloud_log_op_net(String p1, String p2, long p3, long p5)
    {
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->set_pass_canceled()V [access_flags=public final] @ 0x363380
*/

    public final void set_pass_canceled()
    {
        com.baidu.sapi2.SapiWebView.c(this.a, com.baidu.sapi2.SapiWebView.s(this.a));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/baidu/sapi2/SapiWebView$SapiWebViewShell;->unite_verify_result(Ljava/lang/String;)V [access_flags=public final] @ 0x3633a8
*/

    public final void unite_verify_result(String p3)
    {
        this.a.post(new com.baidu.sapi2.SapiWebView$SapiWebViewShell$9(this, p3));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/tencent/bugly/crashreport/crash/h5/H5JavaScriptInterface;->printLog(Ljava/lang/String;)V [access_flags=public] @ 0x584068
*/

    public void printLog(String p4)
    {
        Object[] v1_1 = new Object[1];
        v1_1[0] = p4;
        com.tencent.bugly.proguard.w.d("Log from js: %s", v1_1);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/tencent/bugly/crashreport/crash/h5/H5JavaScriptInterface;->reportJSException(Ljava/lang/String;)V [access_flags=public] @ 0x584094
*/

    public void reportJSException(String p9)
    {
        if (p9 != null) {
            String v0_3 = com.tencent.bugly.proguard.a.c(p9.getBytes());
            if ((this.b == null) || (!this.b.equals(v0_3))) {
                this.b = v0_3;
                java.util.LinkedHashMap v1_1 = new Object[0];
                com.tencent.bugly.proguard.w.d("Handling JS exception ...", v1_1);
                String v0_1 = com.tencent.bugly.crashreport.crash.h5.H5JavaScriptInterface.a(p9);
                if (v0_1 != null) {
                    java.util.LinkedHashMap v1_3 = new java.util.LinkedHashMap();
                    Thread v2_2 = new java.util.LinkedHashMap();
                    if (v0_1.a != null) {
                        v2_2.put("[JS] projectRoot", v0_1.a);
                    }
                    if (v0_1.b != null) {
                        v2_2.put("[JS] context", v0_1.b);
                    }
                    if (v0_1.c != null) {
                        v2_2.put("[JS] url", v0_1.c);
                    }
                    if (v0_1.d != null) {
                        v2_2.put("[JS] userAgent", v0_1.d);
                    }
                    if (v0_1.i != null) {
                        v2_2.put("[JS] file", v0_1.i);
                    }
                    if (v0_1.j != 0) {
                        v2_2.put("[JS] lineNumber", Long.toString(v0_1.j));
                    }
                    v1_3.putAll(v2_2);
                    v1_3.putAll(this.e);
                    v1_3.put("Java Stack", this.d);
                    if (v0_1 != null) {
                        com.tencent.bugly.crashreport.inner.InnerApi.postH5CrashAsync(this.c, v0_1.f, v0_1.g, v0_1.h, v1_3);
                    }
                } else {
                    java.util.LinkedHashMap v1_5 = new Object[0];
                    com.tencent.bugly.proguard.w.d("Failed to parse payload.", v1_5);
                }
            } else {
                java.util.LinkedHashMap v1_6 = new Object[0];
                com.tencent.bugly.proguard.w.d("Same payload from js. Please check whether you\'ve injected bugly.js more than one times.", v1_6);
            }
        } else {
            java.util.LinkedHashMap v1_7 = new Object[0];
            com.tencent.bugly.proguard.w.d("Payload from JS is null.", v1_7);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->gotoPrePage()V [access_flags=public] @ 0x58b030
*/

    public void gotoPrePage()
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.webview.WebViewActivity$2(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->leave()V [access_flags=public] @ 0x58b054
*/

    public void leave()
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.webview.WebViewActivity$3(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->popBackUrl(Ljava/lang/String;)V [access_flags=public] @ 0x58b358
*/

    public void popBackUrl(String p1)
    {
        this.a = p1;
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->popBackUrl(Ljava/lang/String; Z)V [access_flags=public] @ 0x58b370
*/

    public void popBackUrl(String p1, boolean p2)
    {
        this.a = p1;
        this.u = p2;
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->setFinishWhenBackBtnClicked(Z)V [access_flags=public] @ 0x58b38c
*/

    public void setFinishWhenBackBtnClicked(boolean p1)
    {
        this.u = p1;
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->setLeftBtnVisible(Z)V [access_flags=public] @ 0x58b3a4
*/

    public void setLeftBtnVisible(boolean p2)
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.webview.WebViewActivity$4(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->setRightBtn(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x58b3c8
*/

    public void setRightBtn(String p2, String p3)
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.webview.WebViewActivity$5(this, p3, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/WebViewActivity;->setTitle(Ljava/lang/String;)V [access_flags=public] @ 0x58b3ec
*/

    public void setTitle(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.WebViewActivity$6(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/AccountAppealActivity;->call3rdPartyLogin(Ljava/lang/String;)V [access_flags=public] @ 0x58b5c0
*/

    public void call3rdPartyLogin(String p5)
    {
        if (!android.text.TextUtils.isEmpty(p5)) {
            com.google.gson.e v1_1 = new com.google.gson.e();
            String v0_4 = ((com.yxcorp.gifshow.activity.AccountAppealActivity$ThirdPartLogin) v1_1.a(p5, com.yxcorp.gifshow.activity.AccountAppealActivity$ThirdPartLogin));
            this.runOnUiThread(new com.yxcorp.gifshow.activity.AccountAppealActivity$4(this, v0_4.mPlatform, v0_4.mCallback, v1_1));
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/AccountAppealActivity;->selectCountryCode(Ljava/lang/String;)V [access_flags=public] @ 0x58b6bc
*/

    public void selectCountryCode(String p2)
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.AccountAppealActivity$3(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/AccountAppealActivity;->sendSms(Ljava/lang/String; Ljava/lang/String;)V [access_flags=public] @ 0x58b6e0
*/

    public void sendSms(String p2, String p3)
    {
        this.runOnUiThread(new com.yxcorp.gifshow.activity.AccountAppealActivity$2(this, p2, p3));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/FeedbackActivity$a;->onFinish()V [access_flags=public final] @ 0x58b800
*/

    public final void onFinish()
    {
        com.yxcorp.gifshow.activity.FeedbackActivity$a$1 v1_1 = new Object[0];
        com.yxcorp.gifshow.util.ToastUtil.info(com.yxcorp.gifshow.g$j.thanks_for_your_feedback, v1_1);
        this.a.a.post(new com.yxcorp.gifshow.activity.FeedbackActivity$a$1(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/FeedbackActivity;->getDeviceInfo()Ljava/lang/String; [access_flags=public] @ 0x58b8d8
*/

    public String getDeviceInfo()
    {
        com.yxcorp.gifshow.activity.FeedbackActivity$FeedbackInfo v1_1 = new com.yxcorp.gifshow.activity.FeedbackActivity$FeedbackInfo();
        v1_1.mUserId = com.yxcorp.gifshow.d.s.getId();
        v1_1.mAppVersion = com.yxcorp.gifshow.d.h;
        v1_1.mManufacturer = com.yxcorp.gifshow.d.f;
        v1_1.mSystemSDKVersion = android.os.Build$VERSION.SDK_INT;
        v1_1.mSystemBuildVersion = android.os.Build$VERSION.RELEASE;
        v1_1.mSystemDisplay = android.os.Build.DISPLAY;
        v1_1.mSystemCodeName = android.os.Build$VERSION.CODENAME;
        int v0_4 = com.yxcorp.gifshow.d.a();
        String v2_1 = v0_4.getResources().getDisplayMetrics();
        v1_1.mScreenWidth = v2_1.widthPixels;
        v1_1.mScreenHeight = v2_1.heightPixels;
        v1_1.mCpuCores = com.yxcorp.utility.util.d.b();
        v1_1.mCpuFrequency = com.yxcorp.utility.util.d.c();
        v1_1.mTotalMemory = com.yxcorp.utility.util.d.a();
        v1_1.mFreeMemory = com.yxcorp.utility.util.d.d(v0_4);
        v1_1.mTotalRom = com.yxcorp.utility.util.d.a(android.os.Environment.getDataDirectory());
        v1_1.mFreeRom = com.yxcorp.utility.util.d.b(android.os.Environment.getDataDirectory());
        if ("mounted".equals(android.os.Environment.getExternalStorageState())) {
            v1_1.mTotalSdcard = com.yxcorp.utility.util.d.a(android.os.Environment.getExternalStorageDirectory());
            v1_1.mFreeSdcard = com.yxcorp.utility.util.d.b(android.os.Environment.getExternalStorageDirectory());
        }
        v1_1.mCurrentNetworkTypeName = com.yxcorp.utility.util.c.c(v0_4);
        if (com.yxcorp.utility.util.c.e(v0_4)) {
            int v0_11;
            int v0_10 = ((android.net.wifi.WifiManager) v0_4.getSystemService("wifi")).getConnectionInfo();
            if (v0_10 == 0) {
                v0_11 = 0;
            } else {
                v0_11 = v0_10.getSSID();
            }
            v1_1.mCurrentWifiName = v0_11;
        }
        v1_1.mWifiInfos = com.yxcorp.utility.util.WifiUtil.a(com.yxcorp.gifshow.d.a());
        v1_1.mLastBrowsePhotoId = com.yxcorp.gifshow.util.ar.ay();
        return new com.google.gson.e().b(v1_1);
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->bindPhone(Ljava/lang/String;)V [access_flags=public final] @ 0x594d24
*/

    public final void bindPhone(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$4(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->captureCertVideo(Ljava/lang/String;)V [access_flags=public final] @ 0x594d48
*/

    public final void captureCertVideo(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$6(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->exitWebView()V [access_flags=public final] @ 0x594d6c
*/

    public final void exitWebView()
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$17(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->getDeviceInfo(Ljava/lang/String;)V [access_flags=public final] @ 0x594d90
*/

    public final void getDeviceInfo(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$1(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->popBack()V [access_flags=public final] @ 0x594db4
*/

    public final void popBack()
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$16(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->selectCountryPhoneCode(Ljava/lang/String;)V [access_flags=public final] @ 0x594dd8
*/

    public final void selectCountryPhoneCode(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$2(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->sendSMS(Ljava/lang/String;)V [access_flags=public final] @ 0x594dfc
*/

    public final void sendSMS(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$18(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->setPageTitle(Ljava/lang/String;)V [access_flags=public final] @ 0x594e20
*/

    public final void setPageTitle(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$13(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->setTopLeftBtn(Ljava/lang/String;)V [access_flags=public final] @ 0x594e44
*/

    public final void setTopLeftBtn(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$11(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->setTopRightBtn(Ljava/lang/String;)V [access_flags=public final] @ 0x594e68
*/

    public final void setTopRightBtn(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$12(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->showDialog(Ljava/lang/String;)V [access_flags=public final] @ 0x594e8c
*/

    public final void showDialog(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$15(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->showToast(Ljava/lang/String;)V [access_flags=public final] @ 0x594eb0
*/

    public final void showToast(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$14(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->submitData(Ljava/lang/String;)V [access_flags=public final] @ 0x594ed4
*/

    public final void submitData(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$7(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->uploadContacts(Ljava/lang/String;)V [access_flags=public final] @ 0x594ef8
*/

    public final void uploadContacts(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$5(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/gifshow/activity/webview/JsInjectKwai;->verifyThirdPartyLogin(Ljava/lang/String;)V [access_flags=public final] @ 0x594f1c
*/

    public final void verifyThirdPartyLogin(String p2)
    {
        com.yxcorp.utility.e.a(new com.yxcorp.gifshow.activity.webview.JsInjectKwai$3(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->fansTopPay(I J J J J J)V [access_flags=public] @ 0x5f56b8
*/

    public void fansTopPay(int p13, long p14, long p16, long p18, long p20, long p22)
    {
        this.a = com.yxcorp.gifshow.model.response.PaymentConfigResponse$PayProvider.valueOfInt(p13);
        com.yxcorp.plugin.payment.pay.g.a(this, this.a).a(p14, p18, p20, p22, this.c);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->gotoMessage(Ljava/lang/String;)V [access_flags=public] @ 0x5f56fc
*/

    public void gotoMessage(String p7)
    {
        java.util.HashMap v3_1 = new java.util.HashMap();
        v3_1.put("userIds", p7);
        new com.yxcorp.plugin.activity.FansTopActivity$5(this, com.yxcorp.gifshow.http.tools.g.aV, v3_1, new com.yxcorp.plugin.activity.FansTopActivity$4(this), new com.yxcorp.gifshow.util.c.a()).k();
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->isInstalled(I)Z [access_flags=public] @ 0x5f5750
*/

    public boolean isInstalled(int p3)
    {
        boolean v0_2;
        switch (com.yxcorp.plugin.activity.FansTopActivity$7.a[com.yxcorp.gifshow.model.response.PaymentConfigResponse$PayProvider.valueOfInt(p3).ordinal()]) {
            case 1:
                v0_2 = com.yxcorp.utility.util.d.b(this, "com.tencent.mm");
                break;
            case 2:
                v0_2 = com.yxcorp.utility.util.d.b(this, "com.baidu.wallet");
                break;
            case 3:
                v0_2 = com.yxcorp.utility.util.d.b(this, "com.eg.android.AlipayGphone");
                break;
            default:
                v0_2 = 0;
        }
        return v0_2;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->isProviderSupported(I)Z [access_flags=public] @ 0x5f57c8
*/

    public boolean isProviderSupported(int p3)
    {
        int v0_1;
        switch (com.yxcorp.plugin.activity.FansTopActivity$7.a[com.yxcorp.gifshow.model.response.PaymentConfigResponse$PayProvider.valueOfInt(p3).ordinal()]) {
            case 1:
                v0_1 = com.yxcorp.utility.util.d.b(this, "com.tencent.mm");
                break;
            case 2:
            case 3:
                v0_1 = 1;
                break;
            default:
                v0_1 = 0;
        }
        return v0_1;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->popBack(Z)V [access_flags=public] @ 0x5f58cc
*/

    public void popBack(boolean p2)
    {
        this.runOnUiThread(new com.yxcorp.plugin.activity.FansTopActivity$3(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/activity/FansTopActivity;->popBackUrl(Ljava/lang/String;)V [access_flags=public] @ 0x5f58f0
*/

    public void popBackUrl(String p1)
    {
        this.b = p1;
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->bindPhone(Ljava/lang/String;)V [access_flags=public] @ 0x5f5b3c
*/

    public void bindPhone(String p2)
    {
        this.c = p2;
        this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$3(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->cameraShoot(Ljava/lang/String;)V [access_flags=public] @ 0x5f5b64
*/

    public void cameraShoot(String p2)
    {
        this.b = p2;
        this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$1(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->captureCertVideo(Ljava/lang/String;)V [access_flags=public] @ 0x5f5b8c
*/

    public void captureCertVideo(String p6)
    {
        this.a = ((com.yxcorp.gifshow.activity.webview.component.JsVideoCaptureParams) com.yxcorp.gifshow.http.b.a.p.a(p6, com.yxcorp.gifshow.activity.webview.component.JsVideoCaptureParams));
        if (this.a != null) {
            this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$2(this));
        } else {
            String v1_0 = new Object[0];
            com.yxcorp.gifshow.util.ToastUtil.alert(com.yxcorp.gifshow.h.a$h.error, v1_0);
            Object[] v2_1 = new Object[2];
            v2_1[0] = "json";
            v2_1[1] = p6;
            com.yxcorp.gifshow.log.i.b("ks://live_authenticate_web", "parseError", v2_1);
        }
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->deposit(J Ljava/lang/String;)V [access_flags=public] @ 0x5f5c08
*/

    public void deposit(long p2, String p4)
    {
        this.w = p4;
        this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$7(this, p2));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->liveStreamAuthOpened()V [access_flags=public] @ 0x5f5c30
*/

    public void liveStreamAuthOpened()
    {
        com.yxcorp.gifshow.util.ar.a(com.yxcorp.gifshow.plugin.impl.live.LiveStreamStatus.AVAILABLE);
        this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$8(this));
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->uploadContacts(Ljava/lang/String;)V [access_flags=public] @ 0x5f5e44
*/

    public void uploadContacts(String p5)
    {
        com.yxcorp.gifshow.util.bf.a.submit(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$5(this));
        this.l.postDelayed(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$6(this, p5), 100);
        return;
    }


/*
@Landroid/webkit/JavascriptInterface;
Lcom/yxcorp/plugin/live/LiveAuthenticateEntryActivity;->uploadSuccess()V [access_flags=public] @ 0x5f5e84
*/

    public void uploadSuccess()
    {
        this.setFinishWhenBackBtnClicked(1);
        this.runOnUiThread(new com.yxcorp.plugin.live.LiveAuthenticateEntryActivity$4(this));
        Object[] v2_1 = new Object[0];
        com.yxcorp.gifshow.log.i.b("ks://live_authenticate_photo_preview", "upload_photo_success", v2_1);
        return;
    }


