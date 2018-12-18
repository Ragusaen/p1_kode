bin\classifier train "data/training.dataset" --print
bin\classifier test "data/test.dataset" --print
bin\classifier threshold --calc --save
bin\classifier classify "Tidligere FBI-chef langer ud efter Trump og republikanerne: - Der er en sandhed, og de fortæller den ikke"
bin\classifier %*