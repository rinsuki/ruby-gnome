/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_signal.c -

  $Author: sakai $
  $Date: 2002/07/27 14:46:35 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
gobj_sig_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, after;
    ID id = 0;
    int i;
    GClosure* rclosure;

    rb_scan_args(argc, argv, "11", &sig, &after);
    StringValue(sig);

    rclosure = g_rclosure_new(rb_f_lambda());
    i = g_signal_connect_closure(rbgobj_get_gobject(self),
                                 StringValuePtr(sig), rclosure, RTEST(after));

    return INT2FIX(i);
}

/* XXX */
static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig_id;
    VALUE rest;
    GSignalQuery query;
    GValueArray* params;
    GValue return_value = {0,};
    int i;
    VALUE result;

    rb_scan_args(argc, argv, "1*", &sig_id, &rest);
    g_signal_query(NUM2INT(sig_id), &query);

    params = g_value_array_new(query.n_params + 1);
    rbgobj_rvalue_to_gvalue(self, &(params->values[0]));
    for (i = 0; i < query.n_params; i++)
        rbgobj_rvalue_to_gvalue(rb_ary_entry(rest, i), &(params->values[i+1]));

    //g_value_init(&return_value, ); // XXX

    g_signal_emitv(params->values, NUM2INT(sig_id), 0, &return_value);

    g_value_array_free(params);
    result = rbgobj_gvalue_to_rvalue(&return_value);
    g_value_unset(&return_value);
    
    return self;
}

/* TODO */
static VALUE
gobj_sig_emit_by_name(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_notimplement();
    return self;
}

static VALUE
gobj_sig_emit_stop(self, sig_id)
    VALUE self, sig_id;
{
    g_signal_stop_emission(rbgobj_get_gobject(self),
                           NUM2INT(sig_id), 0);
    return self;
}

static VALUE
gobj_sig_emit_stop_by_name(self, sig_name)
    VALUE self, sig_name;
{
    GObject* gobj = rbgobj_get_gobject(self);
    StringValue(sig_name);
    g_signal_stop_emission(gobj,
						   g_signal_lookup(StringValuePtr(sig_name), G_OBJECT_TYPE(gobj)), 0);
    return self;
}

static VALUE
gobj_sig_handler_block(self, id)
	VALUE self, id;
{
    g_signal_handler_block(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_unblock(self, id)
	VALUE self, id;
{
    g_signal_handler_unblock(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_disconnect(self, id)
    VALUE self, id;
{
    g_signal_handler_disconnect(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}


void
Init_gobject_gsignal()
{
    rb_define_method(rbgobj_cGObject, "signal_connect", gobj_sig_connect, -1);
/*
  rb_define_method(rbgobj_cGObject, "signal_connect_after",
  gobj_sig_connect_after, -1);
*/

    rb_define_method(rbgobj_cGObject, "signal_emit",
                     gobj_sig_emit, -1);
    rb_define_method(rbgobj_cGObject, "signal_emit_by_name",
                     gobj_sig_emit_by_name, -1);
    rb_define_method(rbgobj_cGObject, "signal_emit_stop",
                     gobj_sig_emit_stop, 1);
    rb_define_method(rbgobj_cGObject, "signal_emit_stop_by_name",
                     gobj_sig_emit_stop_by_name, 1);
    rb_define_method(rbgobj_cGObject, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rb_define_method(rbgobj_cGObject, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rb_define_method(rbgobj_cGObject, "signal_disconnect",
                     gobj_sig_handler_disconnect, 1);
}
