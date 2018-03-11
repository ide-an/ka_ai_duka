import os
import shutil
import time

deploy_root_dir = "./tmp-deploy/"
sample_script_dir = "./sample-scripts/"
bin_dir = "./Release/"
static_files_dir = "./static-files/"

deploy_name = time.strftime("ka_ai_duka-%Y%m%d-%H%M%S")
deploy_dir = deploy_root_dir + deploy_name + "/"
if not(os.path.exists(deploy_dir)):
  os.makedirs(deploy_dir)

def cp(basename, srcdir):
  shutil.copy(srcdir+basename, deploy_dir)
def cp_r(src, parrent):
  shutil.copytree(parrent+src, deploy_dir+src)

cp("ka_ai_duka.exe", bin_dir)
cp("inject.dll", bin_dir)
cp("setting.exe", bin_dir)
cp_r(sample_script_dir, "")
cp("ka_ai_duka.ini", static_files_dir)
cp("readme.txt", static_files_dir)
cp("LICENSE.txt", static_files_dir)
cp_r("doc", static_files_dir)
