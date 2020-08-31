#define RSS_ITEM_INIT ((struct rss_item) { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL })

struct rss_item {
	const char *title;
	const char *link;
	const char *description;
	const char *author;
	// TODO category
	const char *comments;
	// TODO enclosure
	const char *pubDate;
	const char *source;
};

char *rssgen_item (struct rss_item item) {
	if (item.title == NULL || item.link == NULL || item.description == NULL) {
		return NULL;
	}
	
}
