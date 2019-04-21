#### 1. Добавьте файл tasks.txt рядом с файлом .exe программы.

<br/>Пример входного файла:

100 <br/>
2 <br/>
1	2	0.1	2 <br/>
2	2	0.5	2 <br/>
3	4	0.1	4 <br/>
4	4	0.5	4 <br/>
5	5	0.1	5 <br/>
6	10	0.1	10 <br/>
7	10	0.5	10 <br/>
8	10	1	10 <br/>
9	10	0.8	10 <br/>
10	20	0.2	20 <br/>
11	25	0.4	25 <br/>
12	25	1	25 <br/>
13	50	0.2	50 <br/>

Описание входного файла:
  1) Первая строка - гиперпериод.
  2) Вторая строка - размер фрейма.
  3) Следующие n-строк - описание задач - имеют формат: <br/>
    - Номер задачи   <br/>
    - Период задачи <br/>
    - Длительность задачи (Дробные числа задаются через точку) <br/>
    - Директивный срок задачи <br/>
    (Параметры в строке задаются через 'Tab').
  
#### 2. В случае успешного выполнения программа:
  - Формирует файл output.exe рядом с .exe программы.
  - Вывод сообщение о том, что файл успешно сформирован.

#### 3. В случае неудачного выполнения программа вывод соообщение о том, что необходимо создать этот файл. 

#### 4. Если в output.txt присутствуют слова 'WARNING', это означает, что заданные задачи нельзя распределить по процессору.  <br/> Причиной может быть, например, то, что задачи не успевают выполниться до окончания директивного срока.  <br/> В таком случае необходимо изменить описание задач так, чтобы все слова 'WARNING' пропали, например, уменьшить длительность задач, которые имеют маленький период, так как они мешают другим задачам занять процессор. 
