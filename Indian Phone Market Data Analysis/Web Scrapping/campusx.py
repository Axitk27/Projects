from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
import time

# ChromeDriver path
driver_path = Service(r"D:/Education/Extra/Machine Learning/Project/Advanced Web Scraping/chromedriver-win64/chromedriver.exe")

# Chrome options
chrome_options = Options()
chrome_options.add_experimental_option("excludeSwitches", ["enable-automation", "enable-logging"])
chrome_options.add_argument("--ignore-certificate-errors")
chrome_options.add_argument("--ignore-ssl-errors")
chrome_options.add_argument("start-maximized")

# Start browser
driver = webdriver.Chrome(service=driver_path, options=chrome_options)

# Open Google
driver.get("https://www.ajio.com")
time.sleep(2)

# Search
search_box = driver.find_element(By.NAME, "q")
search_box.send_keys("Campusx")
search_box.send_keys(Keys.ENTER)
time.sleep(2)

# Click first search result
first_result = driver.find_element(By.XPATH, '(//h3)[1]')
first_result.click()

# Wait to load the clicked page
time.sleep(5)

# Close browser
driver.quit()
