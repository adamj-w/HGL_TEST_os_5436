#pragma once

namespace hegel::arch::x86 {

void pic_remap();

void pic_ack(int intno);

}