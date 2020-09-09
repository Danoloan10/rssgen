#ifndef RSSGEN_H
#define RSSGEN_H

#define RSS_ITEM_INIT ((struct rss_item) { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL /*NULL, NULL*/ })

struct rss_item {
	char *title;
	char *link;
	char *description;
	char *author;
	char *comments;
	char *pubDate;
//	struct rss_args *categories;
//	struct rss_args *enclosures;
//	struct rss_args *source;
	char *source;
	char *guid;
};

struct rss_item_list {
	struct rss_item *list;
	size_t len;
};

struct rss_channel {
	char *title;
	char *link;
	char *description;
	// TODO más
	struct rss_item_list items;
};

char *rssgen (const struct rss_channel *chan, size_t n);

#endif
