def func_(retn, args):
  return {
      "args":args,
      "retn":retn
      }

classes = [];
funcs = [];
#float, void, HitType, bool, int, ItemType, ExAttackType
hittests = """
class HittableObject{
    HittableObject(float, float)
    HitType Type()
    float X() 
    float Y() 
    void SetX(float)
    void SetY(float)
    float Width()
    float Height()
    float Radius()
    float Angle()
};
class HittableRect{
    HittableRect(float, float, float, float)
    HitType Type() 
    void SetWidth(float )
    void SetHeight(float)
    float Width() 
    float Height() 
    float Radius() 
    float Angle() 
};
class HittableCircle{
    HittableCircle(float, float, float)
    HitType Type() 
    void SetRadius(float)
    float Width() 
    float Height() 
    float Radius() 
    float Angle() 
};
class HittableRotatableRect {
    HittableRotatableRect(float, float, float, float, float)
    HitType Type()
    void SetWidth(float )
    void SetHeight(float)
    void SetAngle(float )
    float Width()
    float Height()
    float Radius()
    float Angle()
};
class ExAttack{
    unsigned int Id()
    ExAttackType Type()
    float X();
    float Y();
    float Vx()
    float Vy()
    bool Enabled()
    bool Hittable()
    boost::shared_ptr<managed_types::HittableObject> HittableObject()
};
class Player
{
    float X()
    float Y()
    int Life()
    int CardAttackLevel()
    int BossAttackLevel()
    float CurrentCharge()
    float CurrentChargeMax()
    float ChargeSpeed()
    int Combo()
    int SpellPoint()
    raw_types::PlayerCharacter Character()
    bool IsNativeAI()
    float SpeedFast()
    float SpeedSlow()
    boost::shared_ptr<HittableRect> HittableObjectRect()
    boost::shared_ptr<HittableCircle> HittableObjectCircle()
    boost::shared_ptr<HittableRect> HittableObjectForItem()
};
class Enemy
{
    unsigned int Id()
    float X()
    float Y()
    float Vx();
    float Vy();
    bool Enabled()
    bool IsSpirit()
    bool IsActivatedSpirit()
    bool IsBoss()
    bool IsLily()
    bool IsPseudoEnemy()
    boost::shared_ptr<HittableObject> HittableObject()
};
class Bullet
{
    unsigned int Id()
    float X()
    float Y()
    float Vx()
    float Vy()
    bool Enabled()
    void Update()
    boost::shared_ptr<managed_types::HittableObject> HittableObject()
};
class GameSide
{
    Player* Player()
    Bullets& Bullets()
    Enemies& Enemies()
    Items& Items()
    managed_types::ExAttacks& ExAttacks()
    unsigned int RoundWin()
    unsigned int Score()
};
"""#.replace("\n","")
funcs.append( func_("bool", ["HittableObject*", "HittableObject*"]) );
import re
p_class = re.compile(r"(class\s*\w*\s*{(:?\S|\s)*?})",re.M)
res = [ x for x in  p_class.split(hittests) if p_class.match(x)]
p_classname = re.compile(r"class\s(\w*)")
p_method = re.compile(r"(([a-zA-Z:<>&_]+) ([a-zA-Z]+)\(\s*(\w*)\s*\))")
p_contstructor = re.compile(r"\s{2,}([a-zA-Z]+)\((\s*\w*(?:,\s*\w*\s*)*)\)")
for r in res:
  class_obj = {}
  m = p_classname.search(r)
  classname = m.group(1)
  class_obj["name"] = classname
  m = p_contstructor.search(r)
  if m:
    class_obj["constr"] = {
        "name": m.group(1),
        "args": m.group(2).split(", "),
        }
    #print(m.groups())
  class_obj["methods"] = []
  #print(class_obj)
  #print("--")
  pos = 0
  while True:
    m = p_method.search(r, pos)
    if m is None:
      break
    pos = m.end()
    #print(m.groups())
    args = []
    if m.group(4) != "":
      args.append(m.group(4))
    class_obj["methods"].append({
      "name": m.group(3),
      "retn": m.group(2),
      "args": args
      })
  classes.append(class_obj)
  #print(class_obj)
def constr_name(cls):
  return "luaexport_%s_constructor" % cls["name"]
def method_name(mthd, cls):
  return "luaexport_%s_%s" % (cls["name"], mthd["name"])
def type_check(idx, ty):
  if ty == "float" or ty == "short":
    return "::lua_isnumber(ls, %d)" % idx
  else:
    return "::lua_isuserdata(ls, %d)" % idx
def should_userdata(ty):
  return  ty != "float" and ty != "short"

def type_error_msg(ty):
  return "'%s' expected." % ty
def arg_check(args, first_idx=1):
  check_str = ""
  assign_str = ""
  idx = first_idx
  for arg in args:
    check_str += "::luaL_argcheck(ls, %s, %d, \"%s\");\n" % (
        type_check(idx, arg),
        idx,
        type_error_msg(arg)
        )
    if should_userdata(arg):
      assign_str += retrieve_userdata_arg(idx, arg)
    else:
      assign_str += "%s a%d = (%s)%s;\n" % (arg, idx, arg, retrieve_arg(idx, arg))
    idx += 1
  return check_str+"\n"+assign_str
def type_mask(ty):
  return "LUA_TY_"+ty.upper()[:-1]
def should_shared(ty):
  return ty != "Player" and ty != "GameSide"
def retrieve_userdata_arg(idx, ty):
  def retrieve(idx, ty):
    if should_shared(ty):
      return "a%d = &*(((BindData<boost::shared_ptr<%s> >)aa%d).data);\n" % (idx, ty[:-1], idx)
    else:
      return "a%d = ((BindData<%s>)aa%d).data;\n" % (idx, ty, idx)
  res = """
  aa%d = (lua_bind_type)::lua_touserdata(ls, %d);
  %s a%d;
  if(aa%d.ty & %s){
    %s
  }else{
    return ::luaL_argerror(ls, %d, "%s"); 
  }
  """ % (idx, idx, ty, idx, idx, type_mask(ty), retrieve(idx, ty), idx, type_error_msg(ty))
  return res
def retrieve_this(cls_name):
  #is shared ptr?1
  cls_name += "*"
  res = "::luaL_argcheck(ls, %s, %d, \"%s\");\n" % (
      type_check(1, cls_name),
      1,
      type_error_msg(cls_name)
      )
  res += retrieve_userdata_arg(1, cls_name)
  return res;
def retrieve_arg(idx, ty):
  if ty == "float" or ty == "short":
    return "::lua_tonumber(ls, %d)" % idx
  #is shared ptr?1
  pass

def gen_cnstr(cls):
  res = """int %s(lua_State* ls){
  %s
}""" % (constr_name(cls), arg_check(cls["constr"]["args"]))
  #TODO: instantiate
  print(res)

def gen_method(mthd, cls):
  def gen_call(mthd, cls):
    if mthd["retn"] == "void":
      return """
      a1->%s(%s);
      return 0;""" % (method_name(mthd, cls), ", ".join([ "a"+str(x+1) for x in range(len(mthd["args"]))]) )
    elif mthd["retn"][-1] == "&":
      #TODO: retunr iterator
      return """
      %s r = a1->%s(%s);
      return 1;""" % (mthd["retn"], method_name(mthd, cls), ", ".join([ "a"+str(x+1) for x in range(len(mthd["args"]))]) )#TODO: cast and push the result
    else:
      return """
      %s r = a1->%s(%s);
      return 1;""" % (mthd["retn"], method_name(mthd, cls), ", ".join([ "a"+str(x+1) for x in range(len(mthd["args"]))]) )#TODO: cast and push the result
  res = """int %s(lua_State* ls){
  %s
  %s
  %s
}""" % (method_name(mthd, cls), retrieve_this(cls["name"]), arg_check(mthd["args"], 2), gen_call(mthd, cls))
  print(res)

#print(classes)
for cls in classes:
  if "constr" in cls:
    gen_cnstr(cls)
  for mthd in cls["methods"]:
    gen_method(mthd, cls)
#TODO: bind functions

#TODO: register methods and functions
