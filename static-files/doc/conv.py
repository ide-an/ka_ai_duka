# -*- coding: utf-8 -*-
import re

using_p = False
using_item1 = False
using_item2 = False
def isemptyline(line):
  return line.strip() == ""
def convertlink(line):
  p_url = re.compile(r"(http://[a-zA-Z0-9\.\-_/]*)")
  return p_url.sub(r'<a href="\1">\1</a>', line)
def convert(line, is_prev_empty_line):
  global using_p
  global using_item1
  global using_item2
  p_h1 = re.compile(r"\*{1}([^\*].*)")
  p_h2 = re.compile(r"\*{2}([^\*].*)")
  p_h3 = re.compile(r"\*{3}(.*)")
  p_ul1 = re.compile(r"^- (.*)")
  p_ul2 = re.compile(r"^  - (.*)")
  line = convertlink(line)
  m = p_h1.match(line)
  if m:
    return ("<h1>%s</h1>" % m.group(1))
  m = p_h2.match(line)
  if m:
    return ("<h2>%s</h2>" % m.group(1))
  m = p_h3.match(line)
  if m:
    return ("<h3>%s</h3>" % m.group(1))
  m = p_ul1.match(line)
  if m:
    text = "<li>%s</li>" % m.group(1)
    using_item1 = True
    using_item2 = False
    if is_prev_empty_line and not(isemptyline(line)):
      text = "<ul>\n"+text
    return text
  m = p_ul2.match(line)
  if m:
    text = "<li>%s</li>" % m.group(1)
    if not(using_item2):
      text = "<ul>\n"+text
    using_item2 = True
    return text
  using_item1 = False
  using_item2 = False
  if is_prev_empty_line and not(isemptyline(line)):
    using_p = True
    return "<p>"+line[:-1]
  if using_p and isemptyline(line):
    using_p = False
    return "</p>"
  return line

print("""<!doctype html>
<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8">
    <title>花AI塚 AIスクリプト ドキュメント</title>
  </head>
  <body>
""") 
with open("tmp", encoding="utf-8") as f:
  is_prev_empty_line = True
  for l in f:
    print(convert(l, is_prev_empty_line))
    is_prev_empty_line = isemptyline(l)


print("""
  </body>
</html>
""")
