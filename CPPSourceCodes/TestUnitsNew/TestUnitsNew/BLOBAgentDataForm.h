/*
TWOCC 0-9, a-z, A-Z ���� ��밡��.
������ ����߾��� ���� ��������� ���ƾ� �ϰ�,
�ߺ� �� ���� ��� �ؼ��� �ȵȴ�.
���� ANSI�ڵ带 ����ص� �ǰ� ������� �ᵵ �ȴ� ^ ^
��ſ� �ѹ� �ο� �� ���� UNIQUE���� ���񽺰� ���� �� �� ���� ����Ǿ�� �Ѵ�.

�ѹ� ��� �߾��� �ڵ�� ���� ���� ���� UNREG������ �Űܼ� ����϶�.
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