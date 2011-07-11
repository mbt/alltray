/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * AllTrayCttHelper.vala - AllTray CTT helper program
 * Copyright (c) 2011 Michael B. Trausch <mike@trausch.us>
 */
using GLib;

namespace AllTray {
  public class CttHelper : GLib.Object {
    private string[] _args;

    public static int main(string[] args) {
      stdout.printf("CttHelper (stubbed)\n");
      return(1);
    }
  }
}