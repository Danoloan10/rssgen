#define _GNU_SOURCE

#define RSS_HEAD_TAG(__name) "<" __name ">"
#define RSS_FOOT_TAG(__name) "</" __name ">"
#define RSS_WRAP_TAG(__name, __str) RSS_HEAD_TAG(__name) __str RSS_FOOT_TAG(__name)

#define RSS_TAGS_SIZE(__name) ((size_t) (sizeof(RSS_HEAD_TAG(__name)) - 1) + (sizeof(RSS_FOOT_TAG(__name)) - 1))

#define RSS_BUFSIZ 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rssgen.h"

struct rss_args {
	char *args;
	char *body;
};

struct __buf {
	char *str;
	char *ptr;
	size_t len;
};

/**
 * buf: búfer donde imprimir etiqueta
 * ptr: puntero al último sitio del búfer escrito
 * len: tamaño del búfer
 * tag: etiqueta
 * taglen: tamaño de la etiqueta
 */
static int rssgen_print (struct __buf *buf, const char *tag, size_t taglen)
{
	struct __buf _buf = *buf;

	// si el bufer es NULL, hacer crearlo 
	if (_buf.str == NULL) {
		if ((_buf.str = malloc(RSS_BUFSIZ * sizeof(char))) == NULL)
			return -1;
		_buf.ptr = _buf.str;
		_buf.len = RSS_BUFSIZ * sizeof(char);
	}

	// _ptr + taglen: posición del \0
	// agrandamos exponencialmente búfer si no cabe la etiqueta
	while (_buf.len > 0 && ((_buf.ptr + taglen) >= (_buf.str + _buf.len))) {
		char *newbuf = NULL;
		if ((newbuf = realloc(_buf.str, _buf.len*2)) != NULL) {
			_buf.ptr += (newbuf - _buf.str);
			_buf.str = newbuf;
			_buf.len *= 2;
		} else {
			return -1;
		}
	}

	if (_buf.len > 0) {
		strcpy(_buf.ptr, tag);
		_buf.ptr += taglen;
		*buf = _buf;
		return 0;
	} else {
		return -1;
	}
}

static int xml_put_tag  (struct __buf *buf, const char *name, const char *attrs, const char *body)
{
	int ret = -1;
	size_t item = 0;
	char *tag = NULL;
	ssize_t taglen = 0;

	if (name == NULL || buf == NULL) {
		return -1;
	}

	if (attrs == NULL) {
		if (body == NULL)
			taglen = asprintf(&tag, "<%s/>\n", name);
		else
			taglen = asprintf(&tag, "<%s>\n%s\n</%s>\n", name, body, name);
	else {
		if (body == NULL)
			taglen = asprintf(&tag, "<%s %s/>", name, attrs);
		else
			taglen = asprintf(&tag, "<%s %s>%s</%s>", name, attrs, body, name);
	}

	if (taglen >= 0) {
		if (taglen > 0) {
			ret = rssgen_print(buf, tag, taglen);
		}
		free(tag);
	}
	return ret;
}

static char *rssgen_item (struct rss_item *item)
{
	int ret = 0;
	struct __buf buf = { NULL, NULL, 0 };
	struct rss_item _item;

	if (item == NULL)
		return NULL;

	if (item->title != NULL &&
		item->link != NULL &&
		item->description != NULL)
	{
		ret |= xml_put_tag(&buf, "title",       NULL, item->title);
		ret |= xml_put_tag(&buf, "link",        NULL, item->link);
		ret |= xml_put_tag(&buf, "description", NULL, item->description);
		if (item->author != NULL)
			ret |= xml_put_tag(&buf, "author",      NULL, item->author);
		if (item->comments != NULL)
			ret |= xml_put_tag(&buf, "comments",    NULL, item->comments);
		if (item->pubDate != NULL)
			ret |= xml_put_tag(&buf, "pubDate",     NULL, item->pubDate);
		if (item->source != NULL)
			ret |= xml_put_tag(&buf, "source",      NULL, item->source);
	}

	if (ret && buf.str != NULL) {
		free(buf.str);
		buf.str = NULL;
	}

	return buf.str;
}

static char *rssgen_chan (struct rss_channel *chan)
{
	int ret = 0;
	struct __buf buf = { NULL, NULL, 0 };

	if (chan == NULL)
		return NULL;

	if (chan->title != NULL &&
			chan->link != NULL &&
			chan->description != NULL)
	{
		ret |= xml_put_tag(&buf, "title", NULL, chan->title);
		ret |= xml_put_tag(&buf, "link",  NULL, chan->link);
		ret |= xml_put_tag(&buf, "description", NULL, chan->description);
		for (int i = 0; i < chan->items.len && !ret; i++) {
			char *item = rssgen_item(&chan->items.list[i]);
			ret |= xml_put_tag(&buf, "item", NULL, item);
			free(item);
		}
	}

	if (ret && buf.str != NULL) {
		free(buf.str);
		buf.str = NULL;
	}

	return buf.str;
}

char *rssgen (struct rss_channel *chan, size_t n)
{
	struct __buf bufs[2] = {
		{ NULL, NULL, 0 },
		{ NULL, NULL, 0 }
	};

	int ret = 0;

	for (int i = 0; i < n && !ret; i++) {
		ret |= ((bufs[0].str = rssgen_chan(&chan[i])) == NULL);
		if (!ret) {
			ret |= xml_put_tag(&bufs[1], "channel", NULL, bufs[0].str); 
			free(bufs[0].str);
		}
	}
	// buf[1] <- cuerpo de <rss>

	if (!ret) {
		char *head = "<?xml version=\"1.0\" encoding=\"encoding-name\"?>\n";

		bufs[0] = bufs[1];
		bufs[1] = (struct __buf) { NULL, NULL, 0 };

		ret |= rssgen_print(&bufs[1], head, strlen(head));
		ret |= xml_put_tag(&bufs[1], "rss", "version=\"2.0\"", bufs[0].str);

		free(bufs[0].str);
	}
	// buf[1] <- <rss> completo

	if (ret && bufs[1].str != NULL) {
		free(bufs[1].str);
		bufs[1].str = NULL;
	}

	return bufs[1].str;
}
