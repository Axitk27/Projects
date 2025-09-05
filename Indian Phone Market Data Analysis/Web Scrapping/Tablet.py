from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
import time 


driver_path = Service(r"D:/Education/Extra/Machine Learning/Project/Advanced Web Scraping/chromedriver-win64/chromedriver.exe")

# Chrome options
chrome_options = Options()
chrome_options.add_experimental_option("excludeSwitches", ["enable-automation", "enable-logging"])
chrome_options.add_argument("--ignore-certificate-errors")
chrome_options.add_argument("--ignore-ssl-errors")
chrome_options.add_argument("start-maximized")
driver = webdriver.Chrome(service=driver_path, options=chrome_options)

driver.get('https://www.smartprix.com/tablets')

old_height = driver.execute_script('return document.body.scrollHeight')

while True:
    try:
        driver.find_element(by = By.XPATH,value='//*[@id="app"]/main/div[1]/div[2]/div[3]').click()
        time.sleep(1)
        new_height = driver.execute_script('return document.body.scrollHeight')
        
        print(old_height,new_height)

        if new_height == old_height:
            break

        old_height = new_height
    except Exception as e:
        time.sleep(2)
        print(e)
        if new_height == old_height:
            break
    

html = driver.page_source    

with open('tablet.html','w',encoding = 'utf-8') as f:
    f.write(html)
print('I am Done as well')
time.sleep(1000)  # Wait for new content to load