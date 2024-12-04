#!/usr/bin/env swipl

:- use_module(library(apply)).
:- use_module(library(main)).
:- initialization(main, main).

sum([],Result):- Result is 0.
sum([Num|Nums],Result):- sum(Nums,Rest), Result is Num+Rest.

empty("").

main(_):-
  read_string(user_input, _, Content),
  split_string(Content, "\n", "", LinesPlusOne),
  exclude(empty,LinesPlusOne,Lines),
  string_grid(Lines,Grid),
  allCoords(Grid,CoordsToConcat),
  append(CoordsToConcat,Positions),
  maplist(xmas_at(Grid),Positions,SubtotalsOne),
  sum_list(SubtotalsOne,ResultOne),
  maplist(x_mas_at(Grid),Positions,SubtotalsTwo),
  sum_list(SubtotalsTwo,ResultTwo),
  write(ResultOne),
  write(" "),
  write(ResultTwo),
  nl().

string_grid(Lines,Result):- maplist(string_chars,Lines,Result).

allCoords(Grid,Coords):-
  width(Grid,Width),
  height(Grid,Height),
  buildCoord1(Width,ColIdxs),
  maplist(buildCoord2(Height),ColIdxs,Coords).

width([],0).
width([Row|_],Result):- length(Row,Result).

height(Grid,Result):- length(Grid,Result).

buildCoord1(X,Result):- numlist(0,X,Result).

buildCoord2(Y,X,Result):-
  numlist(0,Y,YS),
  maplist(paired(X),YS,Result).

paired(A,B,[A,B]).

xmas_at(Grid,Coord,Result):-
  maplist(xmas(Grid,Coord), [
        [[0, 0], [1, 0], [2, 0], [3, 0]],
        [[0, 0], [1, 1], [2, 2], [3, 3]],
        [[0, 0], [0, 1], [0, 2], [0, 3]],
        [[0, 0], [-1, 1], [-2, 2], [-3, 3]],
        [[0, 0], [-1, 0], [-2, 0], [-3, 0]],
        [[0, 0], [-1, -1], [-2, -2], [-3, -3]],
        [[0, 0], [0, -1], [0, -2], [0, -3]],
        [[0, 0], [1, -1], [2, -2], [3, -3]]],Results),
  sum_list(Results, Result).

xmas(Grid,Coord,Offs,Result):-
  extract_string(Grid,Coord,Offs,String),
  String == "XMAS",
  Result is 1.
xmas(_,_,_,Result):- Result is 0.

x_mas_at(Grid,Coord,Result):-
  x_mas(Grid,Coord,[[-1,-1],[0,0],[1,1]]),
  x_mas(Grid,Coord,[[-1,1],[0,0],[1,-1]]),
  Result is 1.
x_mas_at(_,_,0).

x_mas(Grid,Coord,Offs):-
  extract_string(Grid,Coord,Offs,String),
  String == "MAS".
x_mas(Grid,Coord,Offs):-
  extract_string(Grid,Coord,Offs,String),
  String == "SAM".

extract_string(Grid,Coord,Offs,Result):-
  maplist(plus_coord(Coord),Offs,Positions),
  maplist(multiindex(Grid),Positions,Chars),
  string_chars(Result,Chars).

plus_coord([A,B],[C,D],[E,F]):- E is A + C, F is B + D.

multiindex(_,[ColIdx,_],Result):- ColIdx < 0, Result == '.'.
multiindex(_,[_,RowIdx],Result):- RowIdx < 0, Result == '.'.
multiindex(Grid,[_,RowIdx],Result):-
  height(Grid,Height),
  RowIdx >= Height,
  char_code('.',Code),
  char_code(Result,Code).
multiindex(Grid,[ColIdx,_],Result):-
  width(Grid,Width),
  ColIdx >= Width,
  char_code('.',Code),
  char_code(Result,Code).
multiindex(Grid,[ColIdx,RowIdx],Result):-
  height(Grid,Height),
  width(Grid,Width),
  RowIdx < Height,
  ColIdx < Width,
  RowIdx >= 0,
  ColIdx >= 0,
  nth0(RowIdx,Grid,Row),
  nth0(ColIdx,Row,Result).
