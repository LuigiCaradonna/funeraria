<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('font_sizes', function (Blueprint $table) {
            $table->id();
            $table->unsignedTinyInteger('font_id')->constrained();
            $table->unsignedFloat('size');
            $table->unsignedFloat('uppercase')->nullable();
            $table->unsignedFloat('lowercase')->nullable();
            $table->unsignedFloat('number')->nullable();
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('font_sizes');
    }
};
