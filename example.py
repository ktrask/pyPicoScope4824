#!/usr/bin/python3

import PicoScope4824
ps = PicoScope4824.PicoScope4000()
ps.setChannel( ps.channelB, True, ps.dc, ps.range1V, 0)
ps.setChannel( ps.channelA, True, ps.dc, ps.range5V, 0)
ps.setTriggerConditions()
ps.streaming()
