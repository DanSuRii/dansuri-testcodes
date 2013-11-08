/*
TWOCC 0-9, a-z, A-Z 까지 사용가능.
예전에 사용했었던 값을 사용하지도 말아야 하고,
중복 된 값을 사용 해서도 안된다.
물론 ANSI코드를 사용해도 되고 상수값을 써도 된다 ^ ^
대신에 한번 부여 된 값의 UNIQUE함은 서비스가 종료 될 때 까지 보장되어야 한다.

한번 사용 했었던 코드는 삭제 하지 말고 UNREG쪽으로 옮겨서 등록하라.
*/

REG_DATA(m_dwSaveCoolTime, '00')
REG_DATA(m_stQuickSlotInfo, '01')
REG_DATA(m_btParts, 0x0301)
REG_DATA(m_BaseStatus, '03')

#ifndef UNREG_OLDDATA
#define UNREG_OLDDATA(x, y)
#endif
UNREG_OLDDATA(m_oldDeleted, 0x0302)

#undef UNREG_OLDDATA