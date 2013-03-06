/* LibLL - Library of useful for memory Leak Locate
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <unistd.h>

static int lost_len           = 0;
static GHashTable *hash_alloc = NULL;
static GHashTable *hash_freed = NULL;

struct st_mem
{
	int len;
	char *old;
	char *ptr;
	char *line;
};

static void mem_free(void *data)
{
	struct st_mem *mem = (struct st_mem *)data;
	if(mem != NULL) {
		g_free(mem->old);
		g_free(mem->ptr);
		g_free(mem->line);
		g_free(mem);
		mem = NULL;
	}
}

static void manage_alloc(struct st_mem *mem)
{
	g_hash_table_insert(hash_alloc, mem->ptr, mem);
}

static void manage_free(struct st_mem *mem)
{
	if(g_hash_table_lookup(hash_alloc, mem->ptr) == NULL) {
		g_hash_table_insert(hash_freed, mem->ptr, mem);

	} else {
		g_hash_table_remove(hash_alloc, mem->ptr);
	}
}

static void manage_realloc(struct st_mem *mem)
{
	struct st_mem *old_mem = NULL;

	old_mem = g_hash_table_lookup(hash_alloc, mem->old);

	if(old_mem != NULL) {
		g_hash_table_remove(hash_alloc, mem->old);
		g_hash_table_insert(hash_alloc, mem->ptr, mem);
	}
}

static char *get_string(char *buff, char *init, char *end)
{
	char *aux  = NULL;
	char *aux2 = NULL;

	aux = strstr(buff, init);
	if(aux == NULL)
		return NULL;
	else
		aux++;

	aux2 = strstr(aux, end);
	if(aux2 == NULL)
		return NULL;

	return g_strndup(aux, (aux2 - aux));
}

static void parse_line(char *line)
{
	char *len = NULL;
	struct st_mem *mem = NULL;

	if(line == NULL) {
		printf("parse_line line NULL\n");
		_exit(1);
	}

	mem = g_new0(struct st_mem, 1);
	if(mem == NULL) {
		printf("mem null\n");
		_exit(1);
	}
		
	mem->line = g_strdup(line);
	mem->old  = get_string(line, "{", "}");
	mem->ptr  = get_string(line, "[", "]");
	len       = get_string(line, "(", ")");

	if(len != NULL) {
		mem->len = atoi(len);
		free(len);
	}

	if(strstr(line, "REALLOC:") != NULL) {
		manage_realloc(mem);

	} else if(strstr(line, "ALLOC:") != NULL) {
		manage_alloc(mem);

	} else if(strstr(line, "FREE:") != NULL) {
		manage_free(mem);
	}
}

static void read_file(char *file)
{
	size_t len = 0;
	char *line = NULL;
	FILE *arq  = NULL;

	arq = fopen(file, "r");
	if(arq == NULL) {
		printf("Fail fopen %s\n", file);
		_exit(1);
	}

	while ((getline(&line, &len, arq)) != -1) {
		if(line[0] == '\n')
			continue;

		parse_line(line);
	}

	if(line != NULL) {
		free(line);
	}
}

static void alloc_cb(void *key, void *value, void *data)
{
	struct st_mem *mem = (struct st_mem *)value;

	lost_len += mem->len;
	printf("%s\n", mem->line);
}

static void freed_cb(void *key, void *value, void *data)
{
	struct st_mem *mem = (struct st_mem *)value;

	printf("%s\n", mem->line);
}

static void print_residue(void)
{
	printf("\n\nFINISH EXEC. RESULTS ABOVE:\n\n");

	if(g_hash_table_size(hash_alloc) > 0) {
		printf("Alloc Residue: %d\n", g_hash_table_size(hash_alloc));
		g_hash_table_foreach(hash_alloc, alloc_cb, NULL);
		printf("Lost (%d) bytes\n", lost_len);
	} else {
		printf("Alloc residue is empty\n");
	}
	printf("\n\n");

	if(g_hash_table_size(hash_freed) > 0) {
		printf("Freed Residue: %d\n", g_hash_table_size(hash_freed));
		g_hash_table_foreach(hash_freed, freed_cb, NULL);
	} else {
		printf("Freed residue is empty\n");
	}

	printf("\n\n");
}

static void init(char *file)
{
	hash_alloc = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, mem_free);
	hash_freed = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, mem_free);

	read_file(file);

	print_residue();
}

int main(int argc, char **argv)
{
	if(argc != 2 ) {
		printf("Use: %s <log_file>\n", argv[0]);
		return 1;
	}

	if(g_file_test(argv[1], G_FILE_TEST_EXISTS) == FALSE) {
		printf("Fail access %s\n", argv[1]);
		return 1;
	}

	init(argv[1]);

	return 0;
}
