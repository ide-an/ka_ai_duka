��AI�� �` �����ԉf��AI����c�[��
Version: 1.6
Author: ����(@ide_an)
�Ή��o�[�W����: �����ԉf��ver 1.50a
                �����ԉf��ver 1.00
----------------------------------------

���̃A�v���P�[�V�����͓����ԉf�˂�AI������ł���c�[���ł��B

1. �C���X�g�[��
  zip���𓀂��邾���B����readme��ǂ߂Ă�Ȃ�OK�ł��ˁB

2. �A���C���X�g�[��
  ���W�X�g���͎g�p���Ă��Ȃ��̂ŁA�t�H���_���ƍ폜���邾����OK�ł��B

3. �t�@�C���T�v

  ka_ai_duka.exe
    ���̃t�@�C�������s�����AI�����s�ł����Ԃŉԉf�˂��N�����܂��B
    ���̏�Ԃ�Story mode��Match mode��I�Ԃ�AI���v���C���܂��B
  setting.exe
    ��q��ka_ai_duka.ini��ҏW����c�[���ł��B
  ka_ai_duka.ini
    �ݒ�t�@�C���Bka_ai_duka.exe�����s����O�ɗ\�ߐݒ肵�Ă����܂��B
  inject.dll
    �ԉf�˂̎��s�t�@�C���Ƀ��[�h�����DLL�B
  sample-script
    AI�X�N���v�g�̃T���v���ł��B
      random-walk
      passive-avoid
      shoot-nearest
    ��3��ނ�����܂��B
    ���ꂼ��main.lua�����[�h���Ď��s���܂��B
  doc
    AI�̋L�q�Ɋւ���h�L�������g�ł��B

4. �g����

  �� ���̂����Ƃ��ɔ�����score.dat���o�b�N�A�b�v���Ă������Ƃ𐄏����܂��B

  1) setting.exe���N�����A
       * �ԉf�˂̎��s�t�@�C���̃p�X
       * ���s������AI�̃X�N���v�g�̃p�X
     ��ݒ肷��B
     (�����������΃p�X�Ŏw�肵�ĉ�����)
     �Ⴆ��sample-script\random-walk��AI�𓮂��������ꍇ�́A���s������AI�̃X�N���v�g�̃p�X��
       (��AI�˂̂���t�H���_�̃p�X)\sample-script\random-walk\main.lua
     �ɐݒ肵�܂��B
  2) setting.exe����Aka_ai_duka.exe���N������B
  3) Story mode��Match mode��I�����ăQ�[�����J�n�B
  4) ���Ƃ͂����R�ɁB

5. FAQ

  Q. VsyncPatch��Adonis�ƕ��p�ł��܂���?
  A. �ł��܂���B�Ή��\�薳���ł��B

  Q. �l�b�g�ΐ�Ŏg�p�ł��܂���?
  A. �ł��܂���B����͈Ӑ}�I�Ɏg�p�ł��Ȃ����Ă��܂��B

  Q. ��AI�˂�p���č쐬�������v���C�̃A�b�v���[�h��OK?
  A. �O���c�[�����g�p�������v���C�̃A�b�v���[�h��F�߂Ȃ��ꏊ�ւ̃A�b�v���[�h��NG�ł��B
     ����ȊO�̏ꏊ�ł���΂����炭���Ȃ��Ǝv���܂����A�A�b�v���[�h����ۂɂ�
     (�l�Ԃł͂Ȃ�)AI�ɂ�郊�v���C�ł���|�𖾋L����̂��]�܂����ł��B

  Q. �Ĕz�z���Ă�����?
  A. �ǂ����B �z�z����Ƃ��͕K��LICENSE.txt���������ĉ������B

  Q. �o�O���񂾁B
  Q. �v�]������B
  A. �񍐂��肢���܂��B�A�����@�͈ȉ���3��ނ�����܂��B
      * issue�g���b�J https://bitbucket.org/ide_an/ka_ai_duka/issues
      * �����v���C���[�̃c�[���X���ɓ�����
      * Twitter��@ide_an�ɓ�����

6. TODO

  * EX�A�^�b�N��1P/2P�Ԃ��܂����Ƃ��̑��x�擾

7. ����m�F��

  Windows7(64bit)
  Celeron SU2300 1.20GHz
  RAM 4.00GB

  ���Ƃ炢���ł��ڂ��ڂ������܂��B

8. �X�V����

  2015/05/05 ver1.6�����[�X
    �C��
      LuaJIT����{��Lua�����ɖ߂��B�p�t�H�[�}���X���������邪�W�����C�u�����n�̃o�O������B
      ���@��hitBodyRect�����hitBodyForItem�̃T�C�Y�擾�~�X���C��(hitBodyRect�����ۂ��傫���AhitBodyForItem�����ۂ�菬�����Ȃ��Ă���)�B
      ���f�B�X���Ǝl�G�f�P��EX�A�^�b�N�̑��x�擾�~�X���C���B

  2015/01/29 ver1.5�����[�X
    �C��
      ver1.4�ł�������JIT�L���������̂��o�O�������N������(https://bitbucket.org/ide_an/ka_ai_duka/issue/7)�̂ŏl�X�Ɩ������B

  2015/01/24 ver1.4�����[�X
    �C��
      ver1.2�����ver1.3��table.remove, table.insert, select�����������삵�Ȃ��Ȃ��Ă����̂��C���B

  2015/01/12 ver1.3�����[�X
    �C��
      �˖���EX�A�^�b�N�̍��W���Y���Ă��̂ŏC���B

  2014/12/21 ver1.2�����[�X
    �p�t�H�[�}���X���P
      LuaJIT�𓱓��B���̑������B
    �C��
      �h�L�������g�̃t�H�[�}�b�g������Ă��̂ŏC���B

  2014/05/05 ver1.1�����[�X
    �ǉ�
      �ԉf�ː��i��ver1.00�ɑΉ��B
      Bullet.isErasable��ǉ��A������e���ǂ������ʉ\�ɁB
      ChargeType�����GameSide.chargeType��ǉ��A�`���[�W�^�C�v�̎��ʂ��\�ɁB
    �C��
      hitTest�֐��̎��@vs���[�U�[�̓����蔻�菈���̃o�O���C���B
      GameSide.round_win �� GameSide.roundWin �Ƀt�B�[���h����ύX�B

  2014/04/25 ver1.0�����[�X
