import setuptools

with open("README.md", "r", encoding="utf-8") as fd:
    long_description = fd.read()

# replace relative urls to example files with absolute urls to the main git repo
repo_code_url = "https://git.thm.de/ejhm83/cspm_mecanumwheelcar"
long_description = long_description.replace("](examples/", "]({}/examples/".format(repo_code_url))

setuptools.setup(
    name='mecanumCarPy',
    packages=['mecanumCarPy'],
    version='2.0.0',
    license='GPL-3.0',
    description='Mecanum Car Python Library for controlling a Mecanum Car used in Case Study 2024 THM CCCE Control.',
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='Edmund Jochim',
    author_email='edmund.jochim@iem.thm.de',
    url='https://git.thm.de/ejhm83/cspm_mecanumwheelcar',
    download_url='',
    keywords=['THM', 'CCCE', 'CaseStudy', 'SDK', 'MecanumCar'],
    install_requires=[
        'numpy',
        'logging'
    ],
    python_requires='>=3.6',
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: --- :: GNU General Public License v3 (GPLv3)',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
    ],
)