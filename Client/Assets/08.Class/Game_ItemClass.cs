﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using UnityEngine;

namespace TheLastOne.GameClass
{
    public class Game_ItemClass
    {
        private int id = -1;
        private string name = "";
        private Vector3 pos;
        private Vector3 rotation;
        private bool eat = false;
        private bool draw = false;
        private bool sendPacket = false;
        private bool riding = false;
        private bool explosion = false;
        private bool exp_already = false;   // 차량이 한번 터진 이후 다시 터지는 것을 방지 하기 위하여.
        private int hp = 1;
        private int kind = -1;
        private float Car_kmh = 0.0f;
        public GameObject item;
        public VehicleCtrl car;
        public Rigidbody c_rigidbody;

        public int get_id() { return this.id; }
        public int get_hp() { return this.hp; }
        public string get_name() { return this.name; }
        public Vector3 get_pos() { return this.pos; }
        public Vector3 get_rotation() { return this.rotation; }
        public bool get_eat() { return this.eat; }
        public bool get_explosion() { return this.explosion; }
        public bool get_draw() { return this.draw; }
        public bool get_sendPacket() { return this.sendPacket; }
        public bool get_riding() { return this.riding; }
        public int get_kind() { return this.kind; }
        public float get_kmh() { return this.Car_kmh; }
        public bool get_exp_already() { return this.exp_already; }

        public void set_id(int value) { this.id = value; }
        public void set_hp(int value) { this.hp = value; }
        public void set_name(string value) { this.name = value; }
        public void set_pos(Vector3 value) { this.pos = value; }
        public void set_rotation(Vector3 value) { this.rotation = value; }
        public void set_eat(bool value) { this.eat = value; }
        public void set_riding(bool value) { this.riding = value; }
        public void set_draw(bool value) { this.draw = value; }
        public void set_explosion(bool value) { this.explosion = value; }
        public void set_sendPacket(bool value) { this.sendPacket = value; }
        public void set_kmh(float value) { this.Car_kmh = value; }
        public void set_exp_already(bool value) { this.exp_already = value; }

        public Game_ItemClass(int id, string name, Vector3 pos, Vector3 rotation, bool eat, int hp, int kind, bool exp)
        {
            this.id = id;
            this.name = name;
            this.pos = pos;
            this.rotation = rotation;
            this.eat = eat;
            this.hp = hp;
            this.kind = kind;
            this.riding = false;
            this.car = null;
            this.Car_kmh = 0.0f;
            this.explosion = exp;
            exp_already = false;
        }
    }
}
