third_square([], []).
third_square([H], [H]).
third_square([H1, H2], [H1, H2]).
third_square([H1, H2, H3|T], Res):-
    third_square(T, Res1),
    H3sq is H3*H3,
    append([H1, H2, H3sq], Res1, Res).

