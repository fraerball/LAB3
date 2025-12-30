Проект: Сортировка публикаций

Описание:
Программа генерирует, читает, сортирует и выводит таблицу научных публикаций.


Сборка (Mikefile):
make

Генерация данных:
app.exe --generate N --words=data\words.txt --initials=data\initials.txt --surnames=data\surnames.txt --out=data\gen.csv
N-количество публикаций

Пример сортировки:
  app.exe --sort --in=data\gen.csv --out=data\sorted.csv --view=year --type=asc --method=shaker
  app.exe --sort --in=data\gen.csv --out=data\sorted.csv  --view=citations --type=desc --method=merge

Вывод таблицы в терминал:
app.exe --print --in=data\sorted.csv

Примечание:
Если не указать метод,тип,вид сортировки ,то по умолчанию сортировка будет выполняться следующим образом:по годам, по возрастанию, сортировка перемешиванием.
