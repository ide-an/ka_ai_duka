-- issue 8 error()を呼んでもファイル名や行番号が表示されないケースがある
function main()
  --error("hoge");
  error("hoge", 1);
end
