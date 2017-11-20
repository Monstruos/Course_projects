:-dynamic plane/5.

run():-
     retractall(plane/5),
     consult('RGZdatabase.pl'),
     menu().

menu():-
      repeat,
      write('-----------------------'),nl,nl,
      write('���� ������ � ����������� �������� ���������'),nl,nl,
      write('1��������� ����'),nl,
      write('2��������� ����� ����'),nl,
      write('3-������� ���� (�� ������)'),nl,
      write('4-����� ���� ������ �� ������, ���������� � ��������� 6 ����� � ����������� ���������� ������'),nl,
      write('5������ (� ����������)'),nl,
      write('--------------------------------'),nl,
      write('�������� ����� ����: (1-6) '),
      read(X),
      X>0,
      X<6,
      %process(X),
      X=:=5,!.

process(1):-dbview().
%process(2):-add_iron,!.
%process(3):-remove_iron,!.
%process(4):-db_save_iron,!.
%process(5):-find_iron,!.
%process(6):-retractall(iron/3),!.

dbview():-
    write('��������:'), nl,
    plane(Flight, City, time(Hour, Min), date(Day, Month), Cost),
    write('����:'), write(Flight), nl,
    write('�����:'), write(City), nl,
    write('�����:'), write(Hour), write(':'), write(Min), nl,
    write('����:'), write(Day), write('-'), write(Month), nl,
    write('���������:'), write(Cost), nl.

add():-
    write('�������� ����:'), nl,
    repeat,
    write('����� �����: '),
    read(Flight),
    write('�����: '),
    read(City),
    write('����� (���): '),
    read(Hour),
    write('����� (������): '),
    read(Min),
    write('���� (����): '),
    read(Day),
    write('���� (�����): '),
    read(Month),
    write('���������: '),
    read(Cost),
    assertz(plane(Flight, City, time(Hour, Min), date(Day, Month), Cost)),
    next(),!.

next():-
    write('�����? (q ��� ������)'),
    read(A),
    exit(A).

exit(_):-fail.
exit(q).

delete_plane():-
    write('�������� �����(�� ������):'), nl,
    repeat,
    write('������� �����:'),
    read(F),
    retract(plane(F, _, _, _, _)),
    next(),!.


find():-
    write('������� ����� (���)'),
    read(T1),
    write('������� ����� (������)'),
    read(T2),
    write('������� ���� (����)'),
    read(D1),
    write('������� ���� (�����)'),
    read(D2),
    list(time(T1, T2), date(D1, D2), L),
    L = [0].

list(time(H, M), date(D, Mon), L):-
    plane(F, City, time(H1, M1), date(D1, Mon1), Cost),
    comp(time(H, M), date(D, Mon), time(H1, M1), date(D1, Mon1), Res),
    Res >= 0,
    Res =< 360,
    write('����:'), write(F), nl,
    write('�����:'), write(City), nl,
    write('�����:'), write(H1), write(':'), write(M1), nl,
    write('����:'), write(D1), write('-'), write(Mon1), nl,
    write('���������:'), write(Cost), nl,
    append(plane(F, City, time(H1, M1), date(D1, Mon1), Cost), L, L).


comp(time(H1, M1), date(D1, M1), time(H2, M2), date(D2, M2), Rest):-
    compd(date(D1, M1), date(D2, M2), Resd),
    handler(Resd, time(H1, M1), time(H2, M2), Rest).

handler(0, time(H1, M1), time(H2, M2), Rest):-
    compt(time(H1, M1), time(H2, M2), Rest).
handler(1, time(H1, M1), time(H2, M2), Rest):-
    Hb is H2 + 24,
    compt(time(H1, M1), time(Hb, M2), Rest).

compt(time(H1, M1), time(H2, M2), R):-
    R is (H2-H1)*60 + (M2-M1).

compd(date(31, 1), date(1, 2), 1).
compd(date(28, 2), date(1, 3), 1).
compd(date(31, 3), date(1, 4), 1).
compd(date(30, 4), date(1, 5), 1).
compd(date(31, 5), date(1, 6), 1).
compd(date(30, 6), date(1, 7), 1).
compd(date(31, 7), date(1, 8), 1).
compd(date(31, 8), date(1, 9), 1).
compd(date(30, 9), date(1, 10), 1).
compd(date(31, 10), date(1, 11), 1).
compd(date(30, 11), date(1, 12), 1).
compd(date(31, 12), date(1, 1), 1).
compd(date(D1, M1), date(D2, M2), R):-
    (M1 \== M2)->
    R is -1;
    R is D2 - D1.

