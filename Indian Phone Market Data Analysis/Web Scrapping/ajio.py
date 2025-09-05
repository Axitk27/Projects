from selenium import webdriver
from selenium.webdriver.chrome.service import Service
import time

# Set up ChromeDriver service
s = Service(r'chromedriver-win64\chromedriver.exe')

# Launch browser
driver = webdriver.Chrome(service=s)
driver.get('https://www.smartprix.com/mobiles')

# Get height of the page
old_height = driver.execute_script('return document.body.scrollHeight')
# print(f"Page height: {height}")

# Scroll to the bottom
counter = 1
while(True):
    driver.execute_script('window.scrollTo(0, document.body.scrollHeight)')
    time.sleep(2)
    new_height = driver.execute_script('return document.body.scrollHeight')
    print(new_height)
    print(old_height)
    print(counter)
    if new_height == old_height:
        break
    
    old_height = new_height
    counter = counter + 1
    

html = driver.page_source    
with open('phone.html','w',encoding = 'utf-8') as f:
    f.write(html)

time.sleep(1000)  # Wait for new content to load

# Close browser
driver.quit()
