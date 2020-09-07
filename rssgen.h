#ifndef RSSGEN_H
#define RSSGEN_H

#define RSS_ITEM_INIT ((struct rss_item) { NULL, NULL, NULL, NULL, NULL, NULL, NULL, /*NULL, NULL*/ })

struct rss_item {
	const char *title;
	const char *link;
	const char *description;
	const char *author;
	const char *comments;
	const char *pubDate;
//	const struct rss_args *categories;
//	const struct rss_args *enclosures;
//	const struct rss_args *source;
	const char *source;
};

struct rss_item_list {
	struct rss_item *list;
	size_t len;
};

struct rss_channel {
	const char *title;
	const char *link;
	const char *description;
	// TODO más
	struct rss_item_list items;
};

char *rssgen (struct rss_channel *chan, size_t n);

#endif
