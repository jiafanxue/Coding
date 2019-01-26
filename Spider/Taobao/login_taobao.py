#!/usr/bin/env python
# coding=utf-8

import random
from urllib import request, parse
import time
from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.firefox.firefox_profile import FirefoxProfile

class Taobao:

    def __init__(self):

        # 淘宝个人页面
        self.url='https://i.taobao.com/my_taobao.htm'
        # 淘宝登录页面
        self.login_url = "https://login.taobao.com/member/login.jhtml"
        # 帐号和密码
        self.username = ''
        self.password = ''

        self.proxy = '117.36.103.170'
        self.port = '8118'

        # Chrome浏览器
        # chrome_options = webdriver.ChromeOptions()
        # chrome_options.add_argument('--proxy-server=http://' + self.proxy + self.port)
        # self.driver = webdriver.Chrome(chrome_options=chrome_options)

        # # Firefox浏览器
        profile = FirefoxProfile()
        profile.set_preference("network.proxy.type", 1)
        profile.set_preference("network.proxy.http", self.proxy)
        profile.set_preference("network.proxy.http_port", self.port)
        profile.set_preference("network.proxy.share_proxy_settings", True)
        self.driver = webdriver.Firefox(profile)
        self.driver.implicitly_wait(5)

        # Token
        self._tb_token_ = ''

    def login(self):
        self.driver.get(self.login_url)
        time.sleep(3)

        # 普通登录
        self.driver.find_element_by_id("J_Quick2Static").click()

        # 输入帐号密码
        username = self.driver.find_element_by_id("TPL_username_1")
        username.clear()
        username.send_keys(self.username)
        password = self.driver.find_element_by_id("TPL_password_1")
        password.clear()
        password.send_keys(self.password)

        time.sleep(3)

        while True:
            try:
                # 检验是否需要滑动解锁
                check = self.driver.find_element_by_xpath("//*[@id='nocaptcha']")
                if not check.is_displayed():
                    break

                #定位滑块元素
                source = self.driver.find_element_by_xpath("//*[@id='nc_1_n1z']")
                time.sleep(3)

                #定义鼠标拖放动作
                action = ActionChains(self.driver)
                action.click_and_hold(source).perform()

                for index in range(3):
                    try:
                        action.move_by_offset(100, 0).perform() #平行移动鼠标
                    except Exception as e:
                        print(e)
                        break
                    if(index == 3):
                        action.release()
                        time.sleep(1)
                    else:
                        time.sleep(0.01) #等待停顿时间

                #等待JS认证运行,如果不等待容易报错
                time.sleep(3)
                #查看是否认证成功，获取text值
                text = self.driver.find_element_by_xpath("//*[@id='nc_1__scale_text']/span")
                if text.text.startswith(u'验证通过'):
                    print('成功滑动')
                    break
                if text.text.startswith(u'请点击'):
                    print('成功滑动')
                    break
                if text.text.startswith(u'请按住'):
                    continue
            except Exception as e:
                print(e)
                self.driver.find_element_by_xpath("//div[@id='nocaptcha']/div/span/a").click()

        res = ""
        while True:
            url = self.driver.current_url
            if not url.startswith("https://login.taobao.com"):
                cookies = self.driver.get_cookies()
                for cookie in cookies:
                    res += cookie.get('name')+'='+cookie.get('value')+';'
                res = res[:-1]
                break

            try:
                self.driver.find_element_by_id("J_SubmitStatic").click()
            except Exception as e:
                print(e)

            time.sleep(3)

        self.driver.quit()
        return res

    # cookie登录
    def getInfo(self, cookie):
        try:
            headers = {
                'Host': 'i.taobao.com',
                'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:53.0) Gecko/20100101 Firefox/53.0',
                'Accept': '*/*',
                'Referer': 'https://www.taobao.com/',
                'Connection': 'keep-alive',
                'Cookie': cookie
            }
            req = request.Request(url = self.url, headers = headers)
            response = request.urlopen(req)
            html = response.read().decode()

            # 获取信息的各种操作
            print("获取到各种信息...")
            with open("rsp.html","w",encoding="utf-8") as f:
                f.write(html)
        except:
            print("获取信息失败...")

    def main(self):
        cookie = self.login()
        self.getInfo(cookie)

taobao = Taobao()
taobao.main()
