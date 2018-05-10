﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class WinSceneCtrl : MonoBehaviour {

    public Text playerIdText;


    private void Start()
    {
        playerIdText.text = SingletonCtrl.Instance_S.PlayerID;
    }

    public void LobbyButtonClick()
    {
        SingletonCtrl.Instance_S.NowModeNumber = -1;
        SceneManager.LoadScene("LobbyGameScene");
    }
}
