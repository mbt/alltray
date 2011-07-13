/*
 * alltray-ctt-windowlist.c - Window list
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>

#include <alltray-ctt-windowlist.h>

struct alltray_ctt_windowlist_node {
  Display *dpy;
  Window ctt;
  Window parent;

  struct alltray_ctt_windowlist_node *next;
  struct alltray_ctt_windowlist_node *prev;
};

static struct alltray_ctt_windowlist_node *first;

#define WL_NODE_SIZE ((sizeof(struct alltray_ctt_windowlist_node)))

/**
 * Creates a new windowlist node.
 */
static struct alltray_ctt_windowlist_node *
windowlist_node_new(Display *dpy, Window ctt, Window parent) {
  struct alltray_ctt_windowlist_node *retval = calloc(1, WL_NODE_SIZE);
  if(retval == NULL) abort();

  retval->dpy = dpy;
  retval->ctt = ctt;
  retval->parent = parent;

  return(retval);
}

/**
 * Frees a node's allocated storage.
 */
static bool
windowlist_node_free(struct alltray_ctt_windowlist_node *node) {
  free(node);
  return(true);
}

/**
 * Finds a node in the list by parent window ID number.
 *
 * Returns NULL if no node with the specified parent ID number exists.
 */
static struct alltray_ctt_windowlist_node *
windowlist_find_by_parent_window(Window parent) {
  struct alltray_ctt_windowlist_node *retval = NULL;
  struct alltray_ctt_windowlist_node *cur = first;

  while(cur != NULL) {
    if(cur->parent == parent) break;
    cur = cur->next;
  }

  return(cur);
}

/**
 * Finds a node in the list by CTT window ID number.
 *
 * Returns NULL if no node with the specified CTT ID number exists.
 */
static struct alltray_ctt_windowlist_node *
windowlist_find_by_ctt_window(Window ctt) {
  struct alltray_ctt_windowlist_node *retval = NULL;
  struct alltray_ctt_windowlist_node *cur = first;

  while(cur != NULL) {
    if(cur->ctt == ctt) break;
    cur = cur->next;
  }

  return(cur);
}

/**
 * Appends a new node to the windowlist.
 */
static bool
windowlist_append(struct alltray_ctt_windowlist_node *node) {
  if(first == NULL) {
    first = node;
    first->prev = NULL;
    first->next = NULL;
  } else {
    struct alltray_ctt_windowlist_node *cur = first;
    while(cur->next != NULL) cur = cur->next;
    cur->next = node;
    node->prev = cur;
    node->next = NULL;
  }

  return(true);
}

/**
 * Removes a node from the windowlist.
 */
static bool
windowlist_remove(struct alltray_ctt_windowlist_node *node) {
  struct alltray_ctt_windowlist_node *cur = first;

  while((cur != node) && (cur != NULL)) cur = cur->next;
  if(cur == NULL) return(true);

  struct alltray_ctt_windowlist_node *previous, *next;
  previous = cur->prev;
  next = cur->next;

  if(previous != NULL) previous->next = cur->next;
  if(next != NULL) next->prev = cur->prev;

  // If it is the only node, clear the start-of-list pointer.
  if((previous == NULL) && (next == NULL)) first = NULL;

  return(true);
}

int
alltray_ctt_windowlist_len() {
  int len = 0;
  if(first != NULL) {
    struct alltray_ctt_windowlist_node *cur = first;
    while(cur != NULL) {
      len++;
      cur = cur->next;
    }
  }

  return(len);
}

bool
alltray_ctt_windowlist_add(Display *dpy, Window ctt, Window parent) {
  struct alltray_ctt_windowlist_node *new_node = windowlist_node_new(dpy, ctt,
								     parent);
  return(windowlist_append(new_node));
}

bool
alltray_ctt_windowlist_del(Window parent) {
  struct alltray_ctt_windowlist_node *node =
    windowlist_find_by_parent_window(parent);

  if(node != NULL) {
    windowlist_remove(node);
    windowlist_node_free(node);
  }

  return(true);
}

Window
alltray_ctt_windowlist_get_ctt_for_parent(Window parent) {
  Window retval = 0;

  struct alltray_ctt_windowlist_node *node =
    windowlist_find_by_parent_window(parent);

  if(node != NULL) retval = node->ctt;

  return(retval);
}

Window
alltray_ctt_windowlist_get_parent_for_ctt(Window ctt) {
  Window retval = 0;

  struct alltray_ctt_windowlist_node *node =
    windowlist_find_by_ctt_window(ctt);

  if(node != NULL) retval = node->parent;

  return(retval);
}

bool
alltray_ctt_windowlist_get_all_parents(int *count, Window **list) {
  if(*list != NULL) return(false);
  int i = 0;

  *count = alltray_ctt_windowlist_len();
  *list = calloc(*count, sizeof(Window));
  if(*list == NULL) abort();

  struct alltray_ctt_windowlist_node *cur = first;
  while(cur != NULL) {
    (*list)[i] = cur->parent;
    i++;
    cur = cur->next;
  }

  return(true);
}
