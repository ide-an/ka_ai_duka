print("#include \"bind-gen.h\"");
print("#include \"GameSide.h\"");
print("namespace ka_ai_duka{")
print("using namespace ka_ai_duka::managed_types;")
print("""
#define LUA_TY_HITTABLEOBJECT 0x1
#define LUA_TY_HITTABLERECT   0x3
#define LUA_TY_HITTABLECIRCLE 0x5
#define LUA_TY_HITTABLEROTATABLERECT 0x9
#define LUA_TY_EXATTACK 0x10
#define LUA_TY_PLAYER 0x20
#define LUA_TY_ENEMY 0x40
#define LUA_TY_BULLET 0x80
#define LUA_TY_GAMESIDE 0x100
    """)
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
p_method = re.compile(r"(([a-zA-Z:<>&_\*]+) ([a-zA-Z]+)\(\s*(\w*)\s*\))")
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
  return  ty != "float" and ty != "short" and ty != "int" and ty != "bool" and ty != "HitType" and ty != "ExAttackType" and ty != "raw_types::PlayerCharacter"

def type_error_msg(ty):
  return "'%s' expected." % ty
def arg_check(args, first_idx=1):
  check_str = ""
  assign_str = ""
  idx = first_idx
  for arg in args:
    check_str += "luaL_argcheck(ls, %s, %d, \"%s\");\n" % (
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
  return ty != "Player*" and ty != "GameSide*"
def retrieve_userdata_arg(idx, ty):
  def retrieve(idx, ty):
    if should_shared(ty):
      return "a%d = &*(((BindData<boost::shared_ptr<%s> >*)aa%d)->data);\n" % (idx, ty[:-1], idx)
    else:
      return "a%d = ((BindData<%s>*)aa%d)->data;\n" % (idx, ty, idx)
  res = """BindData<char*>* aa%d = (BindData<char*>*)::lua_touserdata(ls, %d);
  %s a%d;
  if(aa%d->ty & %s){
    %s
  }else{
    return ::luaL_argerror(ls, %d, "%s"); 
  }
  """ % (idx, idx, ty, idx, idx, type_mask(ty), retrieve(idx, ty), idx, type_error_msg(ty))
  return res
def retrieve_this(cls_name):
  #is shared ptr?1
  cls_name += "*"
  res = "luaL_argcheck(ls, %s, %d, \"%s\");\n" % (
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
  #print(res)
def metatable_name(ty):
  return ty

def export_primitive(ty, var_name):
  if not(should_userdata(ty)) and ty != "bool":
    return "::lua_pushnumber(ls, %s);" % (var_name, )
  elif ty == "bool":
    return "::lua_pushboolean(ls, %s ? 1 : 0);" % (var_name, )
  elif ty[-1] == "*" or ty.startswith("boost::shared_ptr<"):
    return """BindData<%s >* bind_data = (BindData<%s >*)::lua_newuserdata(ls, sizeof(BindData<%s >));
    bind_data->data = %s;
    ::luaL_newmetatable(ls, "%s");
    ::lua_setmetatable(ls, -2);
    """ % (ty, ty, ty, var_name, metatable_name(ty))

def gen_method(mthd, cls):
  def gen_call(mthd, cls):
    if mthd["retn"] == "void":
      return """a1->%s(%s);
      return 0;""" % (mthd["name"], ", ".join([ "a"+str(x+2) for x in range(len(mthd["args"]))]) )
    elif mthd["retn"][-1] == "&":
      #TODO: retunr iterator
      return """const %s r = a1->%s(%s);
      return 1;""" % (mthd["retn"], mthd["name"], ", ".join([ "a"+str(x+2) for x in range(len(mthd["args"]))]) )#TODO: cast and push the result
    else:
      return """%s r = a1->%s(%s);
      %s
      return 1;""" % (mthd["retn"], mthd["name"], ", ".join([ "a"+str(x+2) for x in range(len(mthd["args"]))]), export_primitive(mthd["retn"], "r"))
  res = """int %s(lua_State* ls){
  %s
  %s
  %s
}""" % (method_name(mthd, cls), retrieve_this(cls["name"]), arg_check(mthd["args"], 2), gen_call(mthd, cls))
  print(res)

def gen_gc_method(cls):
  res = """int %s(lua_State* ls){
  BindData<boost::shared_ptr<%s> >* a1 = (BindData<boost::shared_ptr<%s> >*)::lua_touserdata(ls, 1);
  a1->data.~shared_ptr<%s>();
  return 0;
}""" % (gc_name(cls), cls["name"], cls["name"], cls["name"])
  print(res)

print("""template<typename T>
struct BindData{
  short ty;
  T data;
};
""");


def gc_name(cls):
  return method_name({"name":"gc"}, cls)
def reg_name(cls):
  return "lua_reg_%s" % cls["name"];
#print(classes)
for cls in classes:
  reg = [];
  if "constr" in cls:
    #reg.append((cls["name"], constr_name(cls)))
    gen_cnstr(cls)
  if should_shared(cls["name"]+"*"):
    gen_gc_method(cls)
  for mthd in cls["methods"]:
    reg.append((mthd["name"], method_name(mthd, cls)))
    gen_method(mthd, cls)
  print("""static const luaL_Reg %s[] = {
    %s,
    {NULL, NULL}
  };
  """ % (reg_name(cls), ",\n".join([ "{\""+x[0]+"\","+x[1]+"}" for x in reg]))
  );


#TODO: bind functions

print("void lua_bind_all(::lua_State* ls, PlayerSide player_side){")
  #register methods
for cls in classes:
  print("""::luaL_newmetatable(ls, "%s");
    ::lua_pushstring(ls, "__index");
    ::lua_newtable(ls);
    luaL_register(ls, NULL, %s);
    ::lua_settable(ls, -3);
    """ % (metatable_name(cls["name"]), reg_name(cls)))
  if should_userdata(cls["name"]) and should_shared(cls["name"]+"*"):
    print("""
    ::lua_pushstring(ls, "__gc");
    ::lua_pushcfunction(ls, %s);
    ::lua_settable(ls, -3);
    """ % gc_name(cls));
    print("lua_pop(ls, 1);")
#TODO: register  functions
print("}}")
