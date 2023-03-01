using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UIManager : MonoBehaviour
{
    public static UIManager instance = null;


    private bool isOnSellBtn = false;

    public GameObject monsterShop;
    public GameObject upgradeShop;
    public GameObject skillShop;
    private bool isShowMonsterShop = false;
    private bool isShowUpgradeShop = false;
    private bool isShowSkillShop = false;
    public GameObject OptionWindow;
    private bool isShowOption = false;

    public GameObject monsterStatusWindow;

    public GameObject messageWindow;
    public Text messageText;


    public void ShowRedMessage(string str)
    {
        StopCoroutine("MessageCoroutine");
        Image msgWinImg = messageWindow.GetComponent<Image>();
        msgWinImg.color = new Color(1, 100f / 255f, 100f / 255f, 1);
        messageText.text = str;
        messageText.color = Color.white;
        StartCoroutine("MessageCoroutine");
    }

    public void ShowWhiteMessage(string str)
    {
        StopCoroutine("MessageCoroutine");
        Image msgWinImg = messageWindow.GetComponent<Image>();
        msgWinImg.color = Color.white;
        messageText.text = str;
        messageText.color = new Color(50f / 255f, 50f / 255f, 50f / 255f, 1);
        StartCoroutine("MessageCoroutine");
    }
    IEnumerator MessageCoroutine()
    {
        messageWindow.SetActive(true);
        yield return new WaitForSeconds(1.5f);
        messageWindow.SetActive(false);
    }
    public bool IsOnSell()
    {
        return isOnSellBtn;
    }

    public void SetOnSell(bool isOn)
    {
        isOnSellBtn = isOn;
    }

    public bool IsUIOpen()
    {
        return isShowMonsterShop || isShowUpgradeShop || isShowSkillShop || isShowOption;
    }

    public void ClickMonsterShop()
    {
        isShowUpgradeShop = false;
        isShowSkillShop = false;
        isShowMonsterShop = !isShowMonsterShop;
        monsterShop.SetActive(isShowMonsterShop);
        upgradeShop.SetActive(isShowUpgradeShop);
        skillShop.SetActive(isShowSkillShop);

        SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Button);
    }
    public void ClickUpgradeShop()
    {
        isShowMonsterShop = false;
        isShowSkillShop = false;
        isShowUpgradeShop = !isShowUpgradeShop; 
        monsterShop.SetActive(isShowMonsterShop);
        upgradeShop.SetActive(isShowUpgradeShop);
        skillShop.SetActive(isShowSkillShop);

        SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Button);
    }

    public void ClickSkillShop()
    {
        isShowMonsterShop = false;
        isShowSkillShop = !isShowSkillShop;
        isShowUpgradeShop = false;
        monsterShop.SetActive(isShowMonsterShop);
        upgradeShop.SetActive(isShowUpgradeShop);
        skillShop.SetActive(isShowSkillShop);

        SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Button);
    }

    public void ClickOption()
    {
        isShowMonsterShop = false;
        isShowUpgradeShop = false;
        isShowSkillShop = false;
        monsterShop.SetActive(isShowMonsterShop);
        upgradeShop.SetActive(isShowUpgradeShop);
        skillShop.SetActive(isShowSkillShop);

        isShowOption = !isShowOption;
        OptionWindow.SetActive(isShowOption);

        if (isShowOption == true)
        {
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Pause_In);
        }
        else
        {
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Pause_Out);
        }
    }

    void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            if (instance != this)
                Destroy(this.gameObject);
        }
    }

    private void Start()
    {
        monsterShop.SetActive(false);
        upgradeShop.SetActive(false);
        skillShop.SetActive(false);
        OptionWindow.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if(isShowUpgradeShop || isShowMonsterShop || isShowSkillShop)
            {
                isShowUpgradeShop = false;
                isShowMonsterShop = false;
                isShowSkillShop = false;
                monsterShop.SetActive(isShowMonsterShop);
                upgradeShop.SetActive(isShowUpgradeShop);
                skillShop.SetActive(isShowSkillShop);
            }
            else
            {
                ClickOption();
            }
        }
    }
}
