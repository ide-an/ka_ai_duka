local test_name = ""
local function assertEqual(actual, expect, msg)
  if not msg then
    msg = ""
  end
  if actual ~= expect then
    msg = test_name.." : expect ="..tostring(expect).." but actual = "..tostring(actual).." : "..msg
    error(msg)
  end
end

-- test case for issue #5
local function test_table_insert(prefix)
  test_name = prefix .. "test_table_insert"
  local arr = {1,2,3,4,5}
  table.insert(arr,1,6)
  assertEqual(#arr, 6)
  assertEqual(arr[1], 6)
  assertEqual(arr[2], 1)
  assertEqual(arr[3], 2)
  assertEqual(arr[4], 3)
  assertEqual(arr[5], 4)
  assertEqual(arr[6], 5)
end
local function test_table_remove(prefix)
  test_name = prefix .. "test_table_remove"
  local arr = {1,2,3,4,5}
  local ret = table.remove(arr,1)
  assertEqual(#arr, 4)
  assertEqual(ret, 1)
  assertEqual(arr[1], 2)
  assertEqual(arr[2], 3)
  assertEqual(arr[3], 4)
  assertEqual(arr[4], 5)
end
local function test_select(prefix)
  test_name = prefix .. "test_select"
  local ret = select("#",2,3,4)
  assertEqual(ret, 3)
  ret = select(2,2,3,4)
  assertEqual(ret, 3)
end

test_table_insert("out main ")
test_table_remove("out main ")
test_select("out main ")

function main()
  test_table_insert("in main ")
  test_table_remove("in main ")
  test_select("in main ")
  error("done") -- test succeed!!
end
