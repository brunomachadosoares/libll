libll
=====

A libll (leak locate) foi uma biblioteca desenvolvida com o intuito de descobrir onde havia um memory leak num sistema embarcado./* LibLL - Library of useful for memory leak locate




 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * PSN Team 2010: Bruno M. Soares
 *                Diogo Justen 
 *                Geraldo Signorini
 */

# Historia:
	A LibLL e uma biblioteca criada da necessidade de descobrir um Memory Leak 
de um sistema embarcado de um projeto bem grande.


# Funcionamento:
	A biblioteca redefine as funcoes de alocacao de memoria heap dinamica do
sistema (malloc, calloc, realloc, free) bem como as funcoes utilizadas pela 
GLib-2 para alocacao interna de memoria dinamica.

	Apos a inicializacao da bibiloteca com a funcao "init_lib_ll", para cada
chamada daquele sistema utilizando as funcoes padroes de alocacao de memoria
ou a GLib-2, o biblioteca escrevera em um determinado filedescriptor informacoes
sobre aquela determinaca alocacao ou liberacao de memoria.

	Sempre que possivel, o sistema base onde a biblioteca sera linkada devera
ser compilado com a flag "-rdynamic" ou "-export-dynamic" para que o backtrace
seja mais exato.


# Compilacao e Utilizacao
	A LibLL e gerada para compilacao estatica.
	Basta incluir em sem make file as flags:
	 "-I<dir_h> -L<dir_lib> -lll `pkg-config glib-2.0 --libs --cflags`"

	Obs.: importante lembrar de compilar com a glib-2


TODO: continuar o readme =)

