using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SkillShop : MonoBehaviour
{
    public Text starSkillText;
    public Text starSkillSubText;
    public Text starSkillPrice;

    public Text expSkillText;
    public Text expSkillSubText;
    public Text expSkillPrice;


    public void LevelUpgradeStar()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.starSkillLevel >= 8)
        {
            return;
        }
        int price = GameManager.instance.skillStarPriceList[save.starSkillLevel];
        if (GameManager.instance.monObjectList.Count == 0 && price + 100 > save.MoonPoint && save.MoonPoint >= price)
        {
            //����
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� ��� ����ϸ� ������ ������ �� �����ϴ�");
            return;
        }
        if (price <= save.MoonPoint)
        {
            save.starSkillLevel++;
            save.MoonPoint -= price;
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Unlock);
            GameManager.instance.GameSave();
        }
        else
        {
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
        }
        UpdatePage();
    }

    public void LevelUpgradeExp()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.expSkillLevel >= 8)
        {
            return;
        }
        int price = GameManager.instance.skillExpPriceList[save.expSkillLevel];
        if (GameManager.instance.monObjectList.Count == 0 && price + 100 > save.MoonPoint && save.MoonPoint >= price)
        {
            //����
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� ��� ����ϸ� ������ ������ �� �����ϴ�");
            return;
        }
        if (price <= save.MoonPoint)
        {
            save.expSkillLevel++;
            save.MoonPoint -= price;
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Unlock);
            GameManager.instance.GameSave();
        }
        else
        {
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
        }
        UpdatePage();
    }
    public void UpdatePage()
    {
        SaveData save = GameManager.instance.GetSaveData();


        starSkillText.text = string.Format("�ٴ��ͼ� Lv.{0}", (save.starSkillLevel));
        if (save.starSkillLevel >= 8)
        {
            starSkillSubText.text = string.Format("(�ִ� {0}��)", (save.starSkillLevel));
            starSkillPrice.text = "MAX";
        }
        else
        {
            starSkillSubText.text = string.Format("(�ִ� {0}��)", (save.starSkillLevel));
            starSkillPrice.text = string.Format("{0:n0}", GameManager.instance.skillStarPriceList[save.starSkillLevel]);
        }

        expSkillText.text = string.Format("ôô�ڻ� Lv.{0}", (save.expSkillLevel));
        if (save.expSkillLevel >= 8)
        {
            expSkillSubText.text = string.Format("(�ִ� {0})", (save.expSkillLevel));
            expSkillPrice.text = "MAX";
        }
        else
        {
            expSkillSubText.text = string.Format("(�ִ� {0})", (save.expSkillLevel));
            expSkillPrice.text = string.Format("{0:n0}", GameManager.instance.skillExpPriceList[save.expSkillLevel]);
        }

    }

    void Start()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.starSkillLevel == 0)
        {
            save.starSkillLevel = 1;
        }
        if (save.expSkillLevel == 0)
        {
            save.expSkillLevel = 1;
        }
        UpdatePage();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
