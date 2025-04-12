package com.example.mecanumcar

import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.Button
import android.widget.SeekBar
import android.widget.TextView
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import kotlin.math.round

class MainActivity : AppCompatActivity() {

    private var carIpAddress = "192.168.10.1" // Default IP address
    private var carPort = 4444 // Default port
    private var statusPort = 4445 // Default status port
    private var speed = 64
    private var currentDirection: String? = null
    private val keepAliveInterval = 8000L // 8 seconds
    private val maxRetryAttempts = 3 // Maximum number of retry attempts
    private val handler = Handler(Looper.getMainLooper())
    private lateinit var batteryInfoTextView: TextView
    private lateinit var motorInfoTextView: TextView

    private val keepAliveRunnable = object : Runnable {
        override fun run() {
            CoroutineScope(Dispatchers.Main).launch {
                sendCommand("keepalive")
                //sendCommandWithRetry("keepalive")
            }
            handler.postDelayed(this, keepAliveInterval)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val sharedPrefs = getSharedPreferences("MecanumCarSettings", MODE_PRIVATE)
        carIpAddress = sharedPrefs.getString("carIpAddress", carIpAddress) ?: carIpAddress
        carPort = sharedPrefs.getString("carPort", carPort.toString())?.toInt() ?: carPort
        statusPort = sharedPrefs.getString("statusPort", statusPort.toString())?.toInt() ?: statusPort

        batteryInfoTextView = findViewById(R.id.batteryInfo)
        motorInfoTextView = findViewById(R.id.motorInfo)

        val buttons = mapOf(
            "UP" to findViewById<Button>(R.id.btnUp),
            "DOWN" to findViewById<Button>(R.id.btnDown),
            "LEFT" to findViewById<Button>(R.id.btnLeft),
            "RIGHT" to findViewById<Button>(R.id.btnRight),
            "TOP_LEFT" to findViewById<Button>(R.id.btnTopLeft),
            "TOP_RIGHT" to findViewById<Button>(R.id.btnTopRight),
            "BOTTOM_LEFT" to findViewById<Button>(R.id.btnBottomLeft),
            "BOTTOM_RIGHT" to findViewById<Button>(R.id.btnBottomRight),
            "CW" to findViewById<Button>(R.id.btnCW),
            "CCW" to findViewById<Button>(R.id.btnCCW)
        )

        for ((direction, button) in buttons) {
            button.setOnTouchListener { _, event ->
                when (event.action) {
                    android.view.MotionEvent.ACTION_DOWN -> {
                        currentDirection = direction
                        sendDirectionalCommand(direction)
                        handler.postDelayed(keepAliveRunnable, keepAliveInterval)
                    }
                    android.view.MotionEvent.ACTION_UP,
                    android.view.MotionEvent.ACTION_CANCEL -> {
                        currentDirection = null
                        CoroutineScope(Dispatchers.Main).launch {
                            //sendCommandWithRetry("stop")
                            sendCommand("stop")
                        }
                        handler.removeCallbacks(keepAliveRunnable)
                    }
                }
                true
            }
        }

        val speedSeekBar = findViewById<SeekBar>(R.id.speedSeekBar)
        speedSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                speed = progress
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        startListeningForCarStateUpdates()
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_main, menu)
        return super.onCreateOptionsMenu(menu)
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.settings -> {
                //Toast.makeText(this, "Settings Selected", Toast.LENGTH_SHORT).show()
                val intent = Intent(this, SettingsActivity::class.java)
                startActivity(intent)
                return true
            }
            R.id.about -> {
                //Toast.makeText(this, "About Selected", Toast.LENGTH_SHORT).show()
                showInfoDialog() // Show the info dialog when "ABOUT" is selected
                return true
            }
            R.id.exit -> {
                //Toast.makeText(this, "Exit Selected", Toast.LENGTH_SHORT).show()
                finish() // Close the app when "EXIT" is selected
                return true
            }
        }
        return super.onOptionsItemSelected(item)
    }

    private fun sendDirectionalCommand(direction: String) {
        val command = when (direction) {
            "UP" -> "rc $speed 0 0"
            "DOWN" -> "rc -$speed 0 0"
            "LEFT" -> "rc 0 -$speed 0"
            "RIGHT" -> "rc 0 $speed 0"
            "CW" -> "rc 0 0 $speed"
            "CCW" -> "rc 0 0 -$speed"
            "TOP_LEFT" -> "rc ${round(speed / 2.0).toInt()} -${round(speed / 2.0).toInt()} 0"
            "TOP_RIGHT" -> "rc ${round(speed / 2.0).toInt()} ${round(speed / 2.0).toInt()} 0"
            "BOTTOM_LEFT" -> "rc -${round(speed / 2.0).toInt()} -${round(speed / 2.0).toInt()} 0"
            "BOTTOM_RIGHT" -> "rc -${round(speed / 2.0).toInt()} ${round(speed / 2.0).toInt()} 0"
            else -> "stop"
        }
        sendCommand(command)
    }

    private fun sendCommand(command: String) {
        CoroutineScope(Dispatchers.IO).launch {
            val socket = DatagramSocket()
            val address = InetAddress.getByName(carIpAddress)
            val packet = DatagramPacket(command.toByteArray(), command.length, address, carPort)
            socket.send(packet)
            socket.close()
        }
    }

    private suspend fun sendCommandWithRetry(command: String) {
        var response: String?
        var retryCount = 0 // Initialize retry count
        do {
            response = sendCommandAndWaitForResponse(command)
            if (response == null || response == "error") {
                delay(500) // Wait for 0.5 second before retrying
                retryCount++
            }
        } while ((response == null || response == "error") && retryCount < maxRetryAttempts)
        if (retryCount == maxRetryAttempts) {
            Toast.makeText(this, "Timeout", Toast.LENGTH_SHORT).show()
        }
    }

    private suspend fun sendCommandAndWaitForResponse(command: String): String? {
        return withContext(Dispatchers.IO) {
            val socket = DatagramSocket()
            val address = InetAddress.getByName(carIpAddress)
            val packet = DatagramPacket(command.toByteArray(), command.length, address, carPort)
            socket.send(packet)

            socket.soTimeout = 1000 // 1 second timeout

            return@withContext try {
                val buffer = ByteArray(1024)
                val responsePacket = DatagramPacket(buffer, buffer.size)
                socket.receive(responsePacket)
                val responseMessage = String(responsePacket.data, 0, responsePacket.length)
                responseMessage
            } catch (e: Exception) {
                null
            } finally {
                socket.close()
            }
        }
    }

    private fun startListeningForCarStateUpdates() {
        CoroutineScope(Dispatchers.IO).launch {
            val socket = DatagramSocket(statusPort)
            val buffer = ByteArray(1024)
            while (true) {
                val packet = DatagramPacket(buffer, buffer.size)
                socket.receive(packet)
                val message = String(packet.data, 0, packet.length)
                withContext(Dispatchers.Main) {
                    //updateBatteryInfo(message)
                    //updateMotorInfo(message)
                    updateStatusInfo(message)
                }
            }
        }
    }

    private fun updateStatusInfo(message: String) {
        val regex = """batV:(\d+\.\d+),batP:(\d+),mA:(\d+),mB:(\d+),mC:(\d+),mD:(\d+),""".toRegex()
        val matchResult = regex.find(message)
        if (matchResult != null) {
            val (voltage, percentage, mA, mB, mC, mD) = matchResult.destructured
            batteryInfoTextView.text = "$voltage V \n$percentage %"
            motorInfoTextView.text = "MotorA: $mA % | MotorB: $mB %\nMotorC: $mC % | MotorD: $mD %"
        }
    }

    /*
    private fun updateBatteryInfo(message: String) {
        val regex = """batV:(\d+\.\d+),batP:(\d+),""".toRegex()
        val matchResult = regex.find(message)
        if (matchResult != null) {
            val (voltage, percentage) = matchResult.destructured
            batteryInfoTextView.text = "$voltage V \n$percentage %"
        }
    }
    private fun updateMotorInfo(message: String) {
        val regex = """mA:(\d+),mB:(\d+),mC:(\d+),mD:(\d+)""".toRegex()
        val matchResult = regex.find(message)
        if (matchResult != null) {
            val (mA, mB, mC, mD) = matchResult.destructured
            motorInfoTextView.text = "$mA % | $mB % | $mC % | $mD %"
        }
    }
    */

    private fun showInfoDialog() {
        val builder = AlertDialog.Builder(this)
        builder.setTitle("Info")
        builder.setMessage("Mecanum Wheel Car Wifi Control v1.4\nAuthor: Edmund Jochim\nTHM CCCE Case Study 2024\nLicense:\nApp: GNU GPLv3\nIcons: Apache-2.0\n\n" +
                "THM Logo is the property of Technische Hochschule Mittelhessen")
        builder.setPositiveButton("Close") { dialog, _ ->
            dialog.dismiss()
        }
        builder.create().show()
    }
}
