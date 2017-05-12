/*
	dom.c[pp]

	gSOAP DOM implementation v2

gSOAP XML Web services tools
Copyright (C) 2000-2009, Robert van Engelen, Genivia, Inc. All Rights Reserved.
This part of the software is released under ONE of the following licenses:
GPL, or the gSOAP public license, or Genivia's license for commercial use.
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2009 Robert A. van Engelen, Genivia inc. All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "stdsoap2.h"

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__anyType(struct soap*, struct soap_dom_element const*);
SOAP_FMAC1 void SOAP_FMAC2 soap_default_xsd__anyType(struct soap*, struct soap_dom_element *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__anyType(struct soap*, const struct soap_dom_element *, const char*, const char*);
SOAP_FMAC1 int SOAP_FMAC2 soap_out_xsd__anyType(struct soap*, const char*, int, const struct soap_dom_element *, const char*);
SOAP_FMAC3 struct soap_dom_element * SOAP_FMAC4 soap_get_xsd__anyType(struct soap*, struct soap_dom_element *, const char*, const char*);
SOAP_FMAC1 struct soap_dom_element * SOAP_FMAC2 soap_in_xsd__anyType(struct soap*, const char*, struct soap_dom_element *, const char*);

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__anyAttribute(struct soap*, struct soap_dom_attribute const*);
SOAP_FMAC1 void SOAP_FMAC2 soap_default_xsd__anyAttribute(struct soap*, struct soap_dom_attribute *);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__anyAttribute(struct soap*, const struct soap_dom_attribute *, const char*, const char*);
SOAP_FMAC1 int SOAP_FMAC2 soap_out_xsd__anyAttribute(struct soap*, const char*, int, const struct soap_dom_attribute *, const char*);
SOAP_FMAC3 struct soap_dom_attribute * SOAP_FMAC4 soap_get_xsd__anyAttribute(struct soap*, struct soap_dom_attribute *, const char*, const char*);
SOAP_FMAC1 struct soap_dom_attribute * SOAP_FMAC2 soap_in_xsd__anyAttribute(struct soap*, const char*, struct soap_dom_attribute *, const char*);

#ifdef __cplusplus
extern "C" {
#endif

SOAP_FMAC1 void SOAP_FMAC2 soap_markelement(struct soap*, const void*, int);
SOAP_FMAC1 int SOAP_FMAC2 soap_putelement(struct soap*, const void*, const char*, int, int);
SOAP_FMAC1 void *SOAP_FMAC2 soap_getelement(struct soap*, int*);

#ifdef __cplusplus
}
#endif

/* format string for generating DOM namespace prefixes (<= 16 chars total) */
#define SOAP_DOMID_FORMAT "dom%d"

/* namespace name (URI) lookup and store routines */
static const char *soap_lookup_ns_prefix(struct soap*, const char*);
static const char *soap_push_ns_prefix(struct soap*, const char*, const char*, int);

static int out_element(struct soap *soap, const struct soap_dom_element *node, const char *prefix, const char *name);
static int out_attribute(struct soap *soap, const char *prefix, const char *name, const char *data, const wchar_t *wide, int flag);

/******************************************************************************\
 *
 *	DOM custom (de)serializers
 *
\******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_serialize_xsd__anyType(struct soap *soap, const struct soap_dom_element *node)
{ if (node)
  { if (node->type && node->node)
      soap_markelement(soap, node->node, node->type);
    else
    { const struct soap_dom_element *elt;
      for (elt = node->elts; elt; elt = elt->next)
        soap_serialize_xsd__anyType(soap, elt);
    }
  }
}

/******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_serialize_xsd__anyAttribute(struct soap *soap, const struct soap_dom_attribute *node)
{
}

/******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_default_xsd__anyType(struct soap *soap, struct soap_dom_element *node)
{ node->next = NULL;
  node->prnt = NULL;
  node->elts = NULL;
  node->atts = NULL;
  node->nstr = NULL;
  node->name = NULL;
  node->data = NULL;
  node->wide = NULL;
  node->node = NULL;
  node->type = 0;
  node->head = NULL;
  node->tail = NULL;
  node->soap = soap;
}

/******************************************************************************/

SOAP_FMAC1
void
SOAP_FMAC2
soap_default_xsd__anyAttribute(struct soap *soap, struct soap_dom_attribute *node)
{ node->next = NULL;
  node->nstr = NULL;
  node->name = NULL;
  node->data = NULL;
  node->wide = NULL;
  node->soap = soap;
}

/******************************************************************************/

static int 
out_element(struct soap *soap, const struct soap_dom_element *node, const char *prefix, const char *name)
{ if (node->head && soap_send(soap, node->head))
    return soap->error;
  if (node->type && node->node)
  { if (prefix && *prefix)
    { char *s = (char*)SOAP_MALLOC(soap, strlen(prefix) + strlen(name) + 2);
      if (!s)
        return soap->error = SOAP_EOM;
      sprintf(s, "%s:%s", prefix, name);
      soap_putelement(soap, node->node, s, 0, node->type);
      SOAP_FREE(soap, s);
    }
    else
      return soap_putelement(soap, node->node, name, 0, node->type);
  }
  else if (prefix && *prefix)
  { char *s;
    if (strlen(prefix) + strlen(name) < sizeof(soap->msgbuf))
      s = soap->msgbuf;
    else
    { s = (char*)SOAP_MALLOC(soap, strlen(prefix) + strlen(name) + 2);
      if (!s)
        return soap->error = SOAP_EOM;
    }
    sprintf(s, "%s:%s", prefix, name);
    if (soap_element(soap, s, 0, NULL)) /* element() */
      return soap->error;
    if (s != soap->msgbuf)
      SOAP_FREE(soap, s);
  }
  else if (*name != '-')
    return soap_element(soap, name, 0, NULL); /* element() */
  return soap->error;
}

/******************************************************************************/

static int
out_attribute(struct soap *soap, const char *prefix, const char *name, const char *data, const wchar_t *wide, int flag)
{ char *s;
  const char *t;
  int err;
  if (wide)
    data = soap_wchar2s(soap, wide);
  if (!prefix || !*prefix)
  { if (wide)
      return soap_set_attr(soap, name, data, 2);
    if (flag)
      return soap_set_attr(soap, name, data, 1);
    return soap_attribute(soap, name, data);
  }
  t = strchr(name, ':');
  if (t)
    t++;
  else
    t = name;
  if (strlen(prefix) + strlen(t) < sizeof(soap->msgbuf))
    s = soap->msgbuf;
  else
  { s = (char*)SOAP_MALLOC(soap, strlen(prefix) + strlen(t) + 2);
    if (!s)
      return soap->error = SOAP_EOM;
  } 
  sprintf(s, "%s:%s", prefix, t);
  if (wide)
    err = soap_set_attr(soap, s, data, 2);
  else if (flag)
    err = soap_set_attr(soap, s, data, 1);
  else
    err = soap_attribute(soap, s, data);
  if (s != soap->msgbuf)
    SOAP_FREE(soap, s);
  return err;
}

/******************************************************************************/

SOAP_FMAC1
int
SOAP_FMAC2
soap_out_xsd__anyType(struct soap *soap, const char *tag, int id, const struct soap_dom_element *node, const char *type)
{ if (node)
  { const char *prefix; /* namespace prefix, if namespace is present */
    size_t colon;
    if (!(soap->mode & SOAP_DOM_ASIS))
    { const struct soap_dom_attribute *att;
      for (att = node->atts; att; att = att->next)
      { if (att->name && att->data && !strncmp(att->name, "xmlns:", 6))
	{ if (soap_push_namespace(soap, att->name + 6, att->data) == NULL)
            return soap->error;
	}
        else if (att->name && att->data && !strcmp(att->name, "xmlns"))
	{ if (soap_push_namespace(soap, "", att->data) == NULL)
            return soap->error;
	}
      }
    }
    if (node->name)
      tag = node->name;
    else if (!tag)
      tag = "-";
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node '%s' output at level %u\n", tag, soap->level));
    if ((prefix = strchr(tag, ':')))
    { colon = prefix - tag + 1;
      if (colon > sizeof(soap->tag))
        colon = sizeof(soap->tag);
    }
    else
      colon = 0;
    prefix = NULL;
    if (node->nstr && *node->nstr && !(soap->mode & SOAP_DOM_ASIS))
    { if (colon)
      { strncpy(soap->tag, tag, colon - 1);
        soap->tag[colon - 1] = '\0';
        if ((prefix = soap_push_ns_prefix(soap, soap->tag, node->nstr, 1)) == NULL
         || out_element(soap, node, prefix, tag + colon))
          return soap->error;
      }
      else
      { if ((prefix = soap_lookup_ns_prefix(soap, node->nstr)))
        { if (out_element(soap, node, prefix, tag + colon))
            return soap->error;
        }
        else
	{ if ((prefix = soap_push_ns_prefix(soap, NULL, node->nstr, 1)) == NULL
           || out_element(soap, node, prefix, tag + colon))
            return soap->error;
        }
      }
    }
    else
    { colon = 0;
      if (out_element(soap, node, NULL, tag))
        return soap->error;
    }
    if (!node->type || !node->node)
    { struct soap_dom_attribute *att;
      struct soap_dom_element *elt;
      for (att = node->atts; att; att = att->next)
      { if (att->name)
        { if (att->nstr && !(soap->mode & SOAP_DOM_ASIS))
          { const char *p;
            if ((att->nstr == node->nstr || (node->nstr && !strcmp(att->nstr, node->nstr))) && prefix)
	    { if (out_attribute(soap, prefix, att->name, att->data, att->wide, 0))
	        return soap->error;
	    }
	    else if ((p = soap_lookup_ns_prefix(soap, att->nstr)))
	    { if (out_attribute(soap, p, att->name, att->data, att->wide, 0))
	        return soap->error;
	    }
	    else if (!strncmp(att->name, "xml", 3))
	    { if (out_attribute(soap, NULL, att->name, att->data, att->wide, 0))
                return soap->error;
	    }
	    else if ((p = soap_push_ns_prefix(soap, NULL, att->nstr, 0)) == NULL
	          || out_attribute(soap, p, att->name, att->data, att->wide, 0))
              return soap->error;
          }
	  else if (soap_attribute(soap, att->name, att->wide ? soap_wchar2s(soap, att->wide) : att->data))
            return soap->error;
        }
      }
      if (*tag != '-' && soap_element_start_end_out(soap, NULL))
        return soap->error;
      if (node->data)
      { if (soap_string_out(soap, node->data, 0))
          return soap->error;
      }
      else if (node->wide)
      { if (soap_wstring_out(soap, node->wide, 0))
          return soap->error;
      }
      for (elt = node->elts; elt; elt = elt->next)
      { if (soap_out_xsd__anyType(soap, tag, 0, elt, NULL))
          return soap->error;
      }
      if (node->tail && soap_send(soap, node->tail))
        return soap->error;
      if (!prefix || !*prefix)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "End of DOM node '%s'\n", tag + colon));
        if (soap_element_end_out(soap, tag + colon))
          return soap->error;
      }
      else
      { char *s;
        if (strlen(prefix) + strlen(tag + colon) < sizeof(soap->msgbuf))
	  s = soap->msgbuf;
	else
	{ s = (char*)SOAP_MALLOC(soap, strlen(prefix) + strlen(tag + colon) + 2);
          if (!s)
            return soap->error = SOAP_EOM;
	}
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "End of DOM node '%s'\n", tag));
	sprintf(s, "%s:%s", prefix, tag + colon);
	soap_pop_namespace(soap);
        if (soap_element_end_out(soap, s))
          return soap->error;
        if (s != soap->msgbuf)
	  SOAP_FREE(soap, s);
      }
    }
  }
  return SOAP_OK;
}

/******************************************************************************/

SOAP_FMAC1
int
SOAP_FMAC2
soap_out_xsd__anyAttribute(struct soap *soap, const char *tag, int id, const struct soap_dom_attribute *node, const char *type)
{ if (!(soap->mode & SOAP_DOM_ASIS))
  { const struct soap_dom_attribute *att;
    for (att = node; att; att = att->next)
    { if (att->name && att->data && !strncmp(att->name, "xmlns:", 6))
      { if (soap_push_namespace(soap, att->name + 6, att->data) == NULL)
          return soap->error;
      }
      else if (att->name && att->data && !strcmp(att->name, "xmlns"))
      { if (soap_push_namespace(soap, "", att->data) == NULL)
          return soap->error;
      }
    }
  }
  while (node)
  { if (node->name)
    { if (node->nstr && !(soap->mode & SOAP_DOM_ASIS) && strncmp(node->name, "xml", 3) && !strchr(node->name, ':'))
      { const char *p;
        p = soap_lookup_ns_prefix(soap, node->nstr);
        if (!p && (p = soap_push_ns_prefix(soap, NULL, node->nstr, 1)) == NULL)
          return soap->error;
        if (out_attribute(soap, p, node->name, node->data, node->wide, 1))
          return soap->error;
      }
      else
        out_attribute(soap, NULL, node->name, node->data, node->wide, 1);
    }
    node = node->next;
  }
  return SOAP_OK;
}

/******************************************************************************/

SOAP_FMAC1
struct soap_dom_element *
SOAP_FMAC2
soap_in_xsd__anyType(struct soap *soap, const char *tag, struct soap_dom_element *node, const char *type)
{ register struct soap_attribute *tp;
  register struct soap_dom_attribute **att;
  if (soap_peek_element(soap))
    return NULL;
  if (!node)
  { if (!(node = (struct soap_dom_element*)soap_malloc(soap, sizeof(struct soap_dom_element))))
    { soap->error = SOAP_EOM;
      return NULL;
    }
  }
  soap_default_xsd__anyType(soap, node);
  node->nstr = soap_current_namespace(soap, soap->tag);
  if ((soap->mode & SOAP_DOM_ASIS))
    node->name = soap_strdup(soap, soap->tag);
  else
  { char *s = strchr(soap->tag, ':');
    if (s)
      node->name = soap_strdup(soap, s+1);
    else
      node->name = soap_strdup(soap, soap->tag);
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node '%s' parsed in namespace '%s'\n", node->name, node->nstr?node->nstr:""));
  if ((soap->mode & SOAP_DOM_NODE) || (!(soap->mode & SOAP_DOM_TREE) && *soap->id))
  { if ((node->node = soap_getelement(soap, &node->type)))
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node contains type %d from xsi:type\n", node->type));
      return node;
    }
    if (soap->error == SOAP_TAG_MISMATCH)
      soap->error = SOAP_OK;
    else
      return NULL;
  }
  att = &node->atts;
  for (tp = soap->attributes; tp; tp = tp->next)
  { if (tp->visible)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node attribute='%s' parsed\n", tp->name));
      *att = (struct soap_dom_attribute*)soap_malloc(soap, sizeof(struct soap_dom_attribute));
      if (!*att)
      { soap->error = SOAP_EOM;
        return NULL;
      }
      (*att)->next = NULL;
      (*att)->nstr = soap_current_namespace(soap, tp->name);
      if ((soap->mode & SOAP_DOM_ASIS) || !strncmp(tp->name, "xml", 3))
        (*att)->name = soap_strdup(soap, tp->name);
      else
      { char *s = strchr(tp->name, ':');
        if (s)
          (*att)->name = soap_strdup(soap, s+1);
        else
          (*att)->name = soap_strdup(soap, tp->name);
      }
      if (tp->visible == 2)
        (*att)->data = soap_strdup(soap, tp->value);
      else
        (*att)->data = NULL;
      (*att)->wide = NULL;
      (*att)->soap = soap;
      att = &(*att)->next;
      tp->visible = 0;
    }
  }
  soap_element_begin_in(soap, NULL, 1, NULL);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node '%s' pulled\n", node->name));
  if (soap->body)
  { if (!soap_peek_element(soap))
    { struct soap_dom_element **elt;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node '%s' has subelements\n", node->name));
      elt = &node->elts;
      for (;;)
      { if (!(*elt = soap_in_xsd__anyType(soap, NULL, NULL, NULL)))
        { if (soap->error != SOAP_NO_TAG)
            return NULL;
          soap->error = SOAP_OK;
          break;
        }
	(*elt)->prnt = node;
        elt = &(*elt)->next;
      }
    }
    else if (soap->error == SOAP_NO_TAG)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node '%s' has cdata\n", node->name));
      if ((soap->mode & SOAP_C_UTFSTRING) || (soap->mode & SOAP_C_MBSTRING))
      { if (!(node->data = soap_string_in(soap, 1, -1, -1)))
          return NULL;
      }
      else if (!(node->wide = soap_wstring_in(soap, 1, -1, -1)))
        return NULL;
    }
    else
      return NULL;
    if (soap_element_end_in(soap, node->name))
      return NULL;
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "End of DOM node '%s'\n", node->name));
  }
  return node;
}

/******************************************************************************/

SOAP_FMAC1
struct soap_dom_attribute *
SOAP_FMAC2
soap_in_xsd__anyAttribute(struct soap *soap, const char *tag, struct soap_dom_attribute *node, const char *type)
{ register struct soap_attribute *tp;
  struct soap_dom_attribute *tmp = node;
  struct soap_dom_attribute *att = node;
  for (tp = soap->attributes; tp; tp = tp->next)
  { if (tp->visible)
    { if (!att)
      { att = (struct soap_dom_attribute*)soap_malloc(soap, sizeof(struct soap_dom_attribute));
        if (tmp)
	  tmp->next = att;
	else
	  node = att;
        tmp = att;
      }
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "DOM node attribute='%s'\n", tp->name));
      if (!att)
      { soap->error = SOAP_EOM;
        return NULL;
      }
      att->next = NULL;
      att->nstr = soap_current_namespace(soap, tp->name);
      if ((soap->mode & SOAP_DOM_ASIS) || !strncmp(tp->name, "xml", 3))
        att->name = soap_strdup(soap, tp->name);
      else
      { char *s = strchr(tp->name, ':');
        if (s)
          att->name = soap_strdup(soap, s+1);
	else
          att->name = soap_strdup(soap, tp->name);
      }
      if (tp->visible == 2)
        att->data = soap_strdup(soap, tp->value);
      else
        att->data = NULL;
      att->wide = NULL;
      att->soap = soap;
      att = NULL;
    }
  }
  return node;
}

/******************************************************************************\
 *
 *	DOM traversing
 *
\******************************************************************************/

SOAP_FMAC1
struct soap_dom_element *
SOAP_FMAC2
soap_dom_next_element(struct soap_dom_element *elt)
{ if (elt->elts)
    return elt->elts;
  if (elt->next)
    return elt->next;
  do elt = elt->prnt;
  while (elt && !elt->next);
  if (elt)
    elt = elt->next;
  return elt;
}

/******************************************************************************/

struct soap_dom_attribute *
soap_dom_next_attribute(struct soap_dom_attribute *att)
{ return att->next;
}

/******************************************************************************\
 *
 *	Namespace prefix lookup/store
 *
\******************************************************************************/

static const char *
soap_lookup_ns_prefix(struct soap *soap, const char *ns)
{ register struct soap_nlist *np;
  for (np = soap->nlist; np; np = np->next)
  { if (np->ns && !strcmp(np->ns, ns)) 
      return np->id;
  }
  return NULL;
}

/******************************************************************************/

static const char *
soap_push_ns_prefix(struct soap *soap, const char *id, const char *ns, int flag)
{ register struct soap_nlist *np;
  if (!id)
  { struct Namespace *n;
    for (n = soap->local_namespaces; n && n->id; n++)
    { if (n->ns == ns || !strcmp(n->ns, ns))
      { id = n->id;
        break;
      }
    }
    if (!id)
    { sprintf(soap->tag, SOAP_DOMID_FORMAT, soap->idnum++);
      id = soap->tag;
    }
  }
  /* fix advance generation of xmlns, when element (level) is not output yet */
  if (flag)
    soap->level++;
  np = soap_push_namespace(soap, id, ns);
  if (flag)
    soap->level--;
  if (!np)
    return NULL;
  if (!np->ns)
  { np->ns = soap->local_namespaces[np->index].out;
    if (!np->ns)
      np->ns = soap->local_namespaces[np->index].ns;
  }
  np->index = 0; /* for C14N utilized mark */
  if (*np->id)
  { sprintf(soap->msgbuf, "xmlns:%s", np->id);
    out_attribute(soap, NULL, soap->msgbuf, ns, NULL, flag);
  }
  else
    out_attribute(soap, NULL, "xmlns", ns, NULL, flag);
  return np->id;
}

#ifdef __cplusplus

/******************************************************************************\
 *
 *	soap_dom_element class
 *
\******************************************************************************/

soap_dom_element::soap_dom_element()
{ soap_default_xsd__anyType(NULL, this);
}

/******************************************************************************/

soap_dom_element::soap_dom_element(struct soap *soap)
{ soap_default_xsd__anyType(soap, this);
}

/******************************************************************************/

soap_dom_element::soap_dom_element(struct soap *soap, const char *nstr, const char *name)
{ soap_default_xsd__anyType(soap, this);
  this->nstr = soap_strdup(soap, nstr);
  this->name = soap_strdup(soap, name);
}

/******************************************************************************/

soap_dom_element::soap_dom_element(struct soap *soap, const char *nstr, const char *name, const char *data)
{ soap_default_xsd__anyType(soap, this);
  this->nstr = soap_strdup(soap, nstr);
  this->name = soap_strdup(soap, name);
  this->data = soap_strdup(soap, data);
}

/******************************************************************************/

soap_dom_element::soap_dom_element(struct soap *soap, const char *nstr, const char *name, void *node, int type)
{ soap_default_xsd__anyType(soap, this);
  this->nstr = soap_strdup(soap, nstr);
  this->name = soap_strdup(soap, name);
  this->node = node;
  this->type = type;
}

/******************************************************************************/

soap_dom_element::~soap_dom_element()
{ }

/******************************************************************************/

soap_dom_element &soap_dom_element::set(const char *nstr, const char *name)
{ this->nstr = soap_strdup(soap, nstr);
  this->name = soap_strdup(soap, name);
  return *this;
}

/******************************************************************************/

soap_dom_element &soap_dom_element::set(const char *data)
{ this->data = soap_strdup(soap, data);
  return *this;
}

/******************************************************************************/

soap_dom_element &soap_dom_element::set(void *node, int type)
{ this->node = node;
  this->type = type;
  return *this;
}

/******************************************************************************/

soap_dom_element &soap_dom_element::add(struct soap_dom_element *elt)
{ elt->prnt = this;
  for (struct soap_dom_element *e = elts; e; e = e->next)
  { if (!e->next)
    { e->next = elt;
      return *this;
    }
  }
  elts = elt;
  return *this;
}

/******************************************************************************/

soap_dom_element &soap_dom_element::add(struct soap_dom_element &elt)
{ return add(&elt);
}

/******************************************************************************/

soap_dom_element &soap_dom_element::add(struct soap_dom_attribute *att)
{ for (struct soap_dom_attribute *a = atts; a; a = a->next)
  { if (!a->next)
    { a->next = att;
      return *this;
    }
  }
  atts = att;
  return *this;
}

/******************************************************************************/

soap_dom_element &soap_dom_element::add(struct soap_dom_attribute &att)
{ return add(&att);
}

/******************************************************************************/

soap_dom_element_iterator soap_dom_element::begin()
{ soap_dom_element_iterator iter(this);
  return iter;
}

/******************************************************************************/

soap_dom_element_iterator soap_dom_element::end()
{ soap_dom_element_iterator iter(NULL);
  return iter;
}

/******************************************************************************/

soap_dom_element_iterator soap_dom_element::find(const char *nstr, const char *name)
{ soap_dom_element_iterator iter(this);
  iter.nstr = nstr;
  iter.name = name;
  if (name && soap_tag_cmp(this->name, name))
    return ++iter;
  if (nstr && this->nstr && soap_tag_cmp(this->nstr, nstr))
    return ++iter;
  return iter;
}

/******************************************************************************/

soap_dom_element_iterator soap_dom_element::find(int type)
{ soap_dom_element_iterator iter(this);
  iter.type = type;
  if (this->type != type)
    return ++iter;
  return iter;
}

/******************************************************************************/

void soap_dom_element::unlink()
{ soap_unlink(soap, this);
  soap_unlink(soap, nstr);
  soap_unlink(soap, name);
  soap_unlink(soap, data);
  soap_unlink(soap, wide);
  if (elts)
    elts->unlink();
  if (atts)
    elts->unlink();
  if (next)
    next->unlink();
  node = NULL;
  type = 0;
}

/******************************************************************************\
 *
 *	soap_dom_attribute class
 *
\******************************************************************************/

soap_dom_attribute::soap_dom_attribute()
{ this->soap = NULL;
  this->next = NULL;
  this->nstr = NULL;
  this->name = NULL;
  this->data = NULL;
  this->wide = NULL;
}

/******************************************************************************/

soap_dom_attribute::soap_dom_attribute(struct soap *soap)
{ this->soap = soap;
  this->next = NULL;
  this->nstr = NULL;
  this->name = NULL;
  this->data = NULL;
  this->wide = NULL;
}

/******************************************************************************/

soap_dom_attribute::soap_dom_attribute(struct soap *soap, const char *nstr, const char *name, const char *data)
{ this->soap = soap;
  this->next = NULL;
  this->nstr = soap_strdup(soap, nstr);
  this->name = soap_strdup(soap, name);
  this->data = soap_strdup(soap, data);
  this->wide = NULL;
}

/******************************************************************************/

soap_dom_attribute::~soap_dom_attribute()
{ }

/******************************************************************************/

soap_dom_attribute_iterator soap_dom_attribute::begin()
{ soap_dom_attribute_iterator iter(this);
  return iter;
}

/******************************************************************************/

soap_dom_attribute_iterator soap_dom_attribute::end()
{ soap_dom_attribute_iterator iter(NULL);
  return iter;
}

/******************************************************************************/

soap_dom_attribute_iterator soap_dom_attribute::find(const char *nstr, const char *name)
{ soap_dom_attribute_iterator iter(this);
  iter.nstr = nstr;
  iter.name = name;
  if (name && soap_tag_cmp(this->name, name))
    return ++iter;
  if (nstr && this->nstr && soap_tag_cmp(this->nstr, nstr))
    return ++iter;
  return iter;
}

/******************************************************************************/

void soap_dom_attribute::unlink()
{ soap_unlink(soap, this);
  soap_unlink(soap, nstr);
  soap_unlink(soap, name);
  soap_unlink(soap, data);
  soap_unlink(soap, wide);
  if (next)
    next->unlink();
}

/******************************************************************************\
 *
 *	soap_dom_element_iterator class
 *
\******************************************************************************/

soap_dom_element_iterator::soap_dom_element_iterator()
{ elt = NULL;
  nstr = NULL;
  name = NULL;
  type = 0;
}

/******************************************************************************/

soap_dom_element_iterator::soap_dom_element_iterator(struct soap_dom_element *elt)
{ this->elt = elt;
  nstr = NULL;
  name = NULL;
  type = 0;
}

/******************************************************************************/

soap_dom_element_iterator::~soap_dom_element_iterator()
{ }

/******************************************************************************/

bool soap_dom_element_iterator::operator==(const soap_dom_element_iterator &iter) const
{ return this->elt == iter.elt;
}

/******************************************************************************/

bool soap_dom_element_iterator::operator!=(const soap_dom_element_iterator &iter) const
{ return this->elt != iter.elt;
}

/******************************************************************************/

struct soap_dom_element &soap_dom_element_iterator::operator*() const
{ return *this->elt;
}

/******************************************************************************/

soap_dom_element_iterator &soap_dom_element_iterator::operator++()
{ while (elt)
  { elt = soap_dom_next_element(elt);
    if (!elt)
      break;
    if (name && elt->name)
    { if (!soap_tag_cmp(elt->name, name))
      { if (nstr && elt->nstr)
        { if (!soap_tag_cmp(elt->nstr, nstr))
	    break;
        }
        else
          break;
      }
    }
    else if (type)
    { if (elt->type == type)
        break;
    }
    else
      break;
  }
  return *this;
}

/******************************************************************************\
 *
 *	soap_dom_attribute_iterator class
 *
\******************************************************************************/

soap_dom_attribute_iterator::soap_dom_attribute_iterator()
{ att = NULL;
  nstr = NULL;
  name = NULL;
}

/******************************************************************************/

soap_dom_attribute_iterator::soap_dom_attribute_iterator(struct soap_dom_attribute *att)
{ this->att = att;
  nstr = NULL;
  name = NULL;
}

/******************************************************************************/

soap_dom_attribute_iterator::~soap_dom_attribute_iterator()
{ }

/******************************************************************************/

bool soap_dom_attribute_iterator::operator==(const soap_dom_attribute_iterator &iter) const
{ return this->att == iter.att;
}

/******************************************************************************/

bool soap_dom_attribute_iterator::operator!=(const soap_dom_attribute_iterator &iter) const
{ return this->att != iter.att;
}

/******************************************************************************/

struct soap_dom_attribute &soap_dom_attribute_iterator::operator*() const
{ return *this->att;
}

/******************************************************************************/

soap_dom_attribute_iterator &soap_dom_attribute_iterator::operator++()
{ while (att)
  { att = soap_dom_next_attribute(att);
    if (!att)
      break;
    if (name && att->name)
    { if (!soap_tag_cmp(att->name, name))
      { if (nstr && att->nstr)
        { if (!soap_tag_cmp(att->nstr, nstr))
	    break;
        }
        else
          break;
      }
    }
    else
      break;
  }
  return *this;
}

/******************************************************************************\
 *
 *	I/O
 *
\******************************************************************************/

#ifndef UNDER_CE

std::ostream &operator<<(std::ostream &o, const struct soap_dom_element &e)
{ if (!e.soap)
  { struct soap soap;
    soap_init2(&soap, SOAP_IO_DEFAULT, SOAP_XML_GRAPH);
    soap_serialize_xsd__anyType(&soap, &e);
    soap_begin_send(&soap);
    soap.ns = 2; /* do not dump namespace table */
    soap_out_xsd__anyType(&soap, NULL, 0, &e, NULL);
    soap_end_send(&soap);
    soap_end(&soap);
    soap_done(&soap);
  }
  else
  { std::ostream *os = e.soap->os;
    e.soap->os = &o;
    soap_mode omode = e.soap->omode;
    soap_set_omode(e.soap, SOAP_XML_GRAPH);
    soap_serialize_xsd__anyType(e.soap, &e);
    soap_begin_send(e.soap);
    e.soap->ns = 2; /* do not dump namespace table */
    soap_out_xsd__anyType(e.soap, NULL, 0, &e, NULL);
    soap_end_send(e.soap);
    e.soap->os = os;
    e.soap->omode = omode;
  }
  return o;
}

/******************************************************************************/

std::istream &operator>>(std::istream &i, struct soap_dom_element &e)
{ if (!e.soap)
    e.soap = soap_new();
  std::istream *is = e.soap->is;
  e.soap->is = &i;
  if (soap_begin_recv(e.soap)
   || !soap_in_xsd__anyType(e.soap, NULL, &e, NULL)
   || soap_end_recv(e.soap))
  { /* handle error? Note: e.soap->error is set and app should check */
  }
  e.soap->is = is;
  return i;
}

#endif

#endif
