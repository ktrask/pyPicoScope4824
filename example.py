#!/usr/bin/python3

import PicoScope4824
ps = PicoScope4824.PicoScope4000()
ps.setChannel( ps.channelB, True, ps.dc, ps.range1V, 0)
ps.setDataBuffer( ps.channelB )
ps.setTriggerConditions()
ps.streaming()
