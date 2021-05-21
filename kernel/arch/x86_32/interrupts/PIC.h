#pragma once

namespace hegel::arch::x86 {

void pic_remap(int offset1, int offset2);

void pic_ack(int intno);

void pic_disable();

}