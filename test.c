#include "rssgen.c"

#include <stdio.h>
#include <string.h>

int main()
{
	char *str, *exp = 
		"<item>"
		"<title>titulo</title>"
		"<link>link</link>"
		"<description>desc</description>"
		"</item>";
	struct rss_item item;
	item = RSS_ITEM_INIT;
	item.title = "titulo";
	item.link  = "link";
	item.description = "desc";
	item.author = "yo";
	str = rssgen_item(&item);
	if(str == NULL) fprintf(stderr, "Error: str == NULL");
	else {
		fprintf(stderr, "%s\n", str);
		free(str);
	}

	struct rss_channel chan;

	chan.title = "canal";
	chan.link = "canalink";
	chan.description = "descanal";
	chan.items.list = &item;
	chan.items.len = 1;

	str = rssgen_chan(&chan);
	if(str == NULL) fprintf(stderr, "Error: str == NULL");
	else {
		fprintf(stderr, "%s\n", str);
		free(str);
	}

	str = rssgen(&chan, 1);
	if(str == NULL) fprintf(stderr, "Error: str == NULL");
	else {
		fprintf(stderr, "%s\n", str);
		free(str);
	}
}
