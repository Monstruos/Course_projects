:-dynamic plane/5.

run():-
     retractall(plane/5),
     consult('RGZdatabase.pl'),
     menu().

menu():-
      repeat,
      write('-----------------------'),nl,nl,
      write('База данных с расписанием движения самолетов'),nl,nl,
      write('1–Просмотр базы'),nl,
      write('2–Добавить новый рейс'),nl,
      write('3-Удалить рейс (по номеру)'),nl,
      write('4-Поиск всех рейсов до города, вылетающих в ближайшие 6 часов с минимальной стоимостью билета'),nl,
      write('5–Выход (и сохранение)'),nl,
      write('--------------------------------'),nl,
      write('Выберите пункт меню: (1-5) '),
      read(X),
      X>0,
      X<6,
      process(X),
      X=:=5,!.

process(1):-dbview().
process(2):-add(),!.
process(3):-delete_plane(),!.
process(4):-find(),!.
process(5):-tell('RGZdatabase.pl'), listing(plane), told,!.

dbview():-
    write('Просмотр:'), nl,
    plane(Flight, City, time(Hour, Min), date(Day, Month), Cost),
    write('Рейс:'), write(Flight), nl,
    write('Город:'), write(City), nl,
    write('Время:'), write(Hour), write(':'), write(Min), nl,
    write('Дата:'), write(Day), write('-'), write(Month), nl,
    write('Стоимость:'), write(Cost), nl,
    write('========================'), nl.

add():-
    write('Добавить рейс:'), nl,
    repeat,
    write('Номер рейса: '),
    read(Flight),
    write('Город: '),
    read(City),
    write('Время (час): '),
    read(Hour),
    write('Время (минута): '),
    read(Min),
    write('Дата (день): '),
    read(Day),
    write('Дата (месяц): '),
    read(Month),
    write('Стоимость: '),
    read(Cost),
    assertz(plane(Flight, City, time(Hour, Min), date(Day, Month), Cost)),
    next(),!.


delete_plane():-
    write('Удаление рейса(по номеру):'), nl,
    repeat,
    write('Введите номер:'),
    read(F),
    retract(plane(F, _, _, _, _)),
    next(),!.

next():-
    write('Выход? (q для выхода)'),
    read(A),
    exit(A).

exit(_):-fail.
exit(q).


find():-
    write('Введите город'),
    read(City),
    write('Введите текущее время (час)'),
    read(T1),
    write('Введите текущее время (минута)'),
    read(T2),
    write('Введите текущую дату (день)'),
    read(D1),
    write('Введите текущую дату (месяц)'),
    read(D2),
    list(time(T1, T2), date(D1, D2), City, Lflight),
    print_list(Lflight).




list(time(H, M), date(D, Mon), City, L2):-
    findall(plane(X, City, time(Y, Z), date(W, Q), E), plane(X, City, time(Y, Z), date(W, Q), E), L),
    find_near_flight(time(H, M), date(D, Mon), L, L1),
    minim(L1, L2).


comp(time(H1, M1), date(D1, Mon1), time(H2, M2), date(D2, Mon2), Rest):-
    compd(date(D1, Mon1), date(D2, Mon2), Resd),
    handler(Resd, time(H1, M1), time(H2, M2), Rest).

handler(0, time(H1, M1), time(H2, M2), Rest):-
    compt(time(H1, M1), time(H2, M2), Rest),!.
handler(1, time(H1, M1), time(H2, M2), Rest):-
    Hb is H2 + 24,
    compt(time(H1, M1), time(Hb, M2), Rest),!.

compt(time(H1, M1), time(H2, M2), R):-
    R is (H2-H1)*60 + (M2-M1),!.

compd(date(31, 1), date(1, 2), 1):-!.
compd(date(28, 2), date(1, 3), 1):-!.
compd(date(31, 3), date(1, 4), 1):-!.
compd(date(30, 4), date(1, 5), 1):-!.
compd(date(31, 5), date(1, 6), 1):-!.
compd(date(30, 6), date(1, 7), 1):-!.
compd(date(31, 7), date(1, 8), 1):-!.
compd(date(31, 8), date(1, 9), 1):-!.
compd(date(30, 9), date(1, 10), 1):-!.
compd(date(31, 10), date(1, 11), 1):-!.
compd(date(30, 11), date(1, 12), 1):-!.
compd(date(31, 12), date(1, 1), 1):-!.
compd(date(D1, M1), date(D2, M2), R):-
    (M1 \== M2)->
    R is -1,!;
    R is D2 - D1,!.


find_near_flight(time(_, _), date(_, _), [], []).
find_near_flight(time(H, M), date(D, Mon), [plane(Flight, City, time(Hp, Mp), date(Dp, Monp), Cost)|T],Lr):-
     find_near_flight(time(H, M), date(D, Mon), T, Lr1),
     (comp(time(H, M), date(D, Mon), time(Hp, Mp), date(Dp, Monp), Rest),
     (Rest >= 0,
     Rest =< 360)->
     append([plane(Flight, City, time(Hp, Mp), date(Dp, Monp), Cost)], Lr1, Lr);
     Lr1 = Lr).

minim([plane(Flight, City, time(H, M), date(D, Mon), Cost)|T], L):-
    min([plane(Flight, City, time(H, M), date(D, Mon), Cost)|T], Cost1),
    setof(plane(F, City, time(H1, M1), date(D1, Mon1), Cost1), plane(F, City, time(H1, M1), date(D1, Mon1), Cost1), L).

min([plane(_, _, time(_, _), date(_, _), Cost)|T], Res):-
    min(T, Res),
    Res < Cost,!.
min([plane(_, _, time(_, _), date(_, _), Cost)|_], Cost).




print_list([]):-!.
print_list([plane(Flight, City, time(Hour, Min), date(Day, Month), Cost)|T]):-
     write('Рейс: '), write(Flight), nl,
     write('Город: '), write(City), nl,
     write('Время: '), write(Hour), write(':'), write(Min), nl,
     write('День: '), write(Day), write('-'), write(Month), nl,
     write('Стоимость: '), write(Cost), nl,
     write('-------------------------'), nl,
     print_list(T).
